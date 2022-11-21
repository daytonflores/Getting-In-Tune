/*
 * dma.c
 *
 *  Created on: Nov 17, 2022
 *      Author: dayton.flores
 */

#include "board.h"
#include "dma.h"
#include "tone.h"

/**
 * \def		DCR_EINT
 * \brief	DCR[31] which is Enable interrupt on completion of transfer
 * \detail
 * 		0: No interrupt is generated
 * 		1: Interrupt signal is enabled. It will be raised by
 * 		   completion of a transfer or occurrence of
 * 		   error condition
 */
#define DCR_EINT\
	(1)

/**
 * \def		DCR_ERQ
 * \brief	DCR[30] which is Enable Peripheral Request
 * \detail
 * 		0: Peripheral request is ignored
 * 		1: Enables peripheral request to initiate transfer.
 * 		   A software-initiated request (i.e. setting the
 * 		   START bit) is always enabled
 */
#define DCR_ERQ\
	(1)

/**
 * \def		DCR_CS
 * \brief	DCR[29] which is Cycle Steal
 * \detail
 * 		0: DMA continuously makes read/write transfers until
 * 		   the BCR decrements to 0
 * 		1: Forces a single read/write transfer per request
 */
#define DCR_CS\
	(1)

/**
 * \def		DCR_SINC
 * \brief	DCR[22] which is Source Increment
 * \detail
 * 		0: No change to SAR after successful transfer
 * 		1: SAR increments by 1/2/4 (as determined by
 * 		   transfer size) after successful transfer
 */
#define DCR_SINC\
	(1)

/**
 * \def		DCR_SSIZE
 * \brief	DCR[21:20] which is Source Size
 * \detail
 * 		00: 32-bit data size of source bus cycle for DMA controller
 * 		01: 8-bit data size of source bus cycle for DMA controller
 * 		10: 16-bit data size of source bus cycle for DMA controller
 * 		11: Reserved
 */
#define DCR_SSIZE\
	(2)

/**
 * \def		DCR_DINC
 * \brief	DCR[19] which is Destination Increment
 * \detail
 * 		0: No change to DAR after successful transfer
 * 		1: DAR increments by 1/2/4 (as determined by
 * 		   transfer size) after successful transfer
 */
#define DCR_DINC\
	(1)

/**
 * \def		DCR_DSIZE
 * \brief	DCR[18:17] which is Destination Size
 * \detail
 * 		00: 32-bit data size of destination bus cycle for DMA controller
 * 		01: 8-bit data size of destination bus cycle for DMA controller
 * 		10: 16-bit data size of destination bus cycle for DMA controller
 * 		11: Reserved
 */
#define DCR_DSIZE\
	(2)

/**
 * \def		CHCFG_SOURCE
 * \brief	CHCFG[5:0] which is DMA Channel Source
 * \detail
 * 		0    : Disabled
 * 		2-7  : UART0,1,2 (Receive, Transmit)
 * 		16-19: SPI0, 1 (Receive, Transmit)
 * 		22-23: I2C0, 1
 * 		24-29: TPM0 (Channels 0-5)
 * 		32-35: TPM1-2 (Channels 0-1)
 * 		40   : ADC0
 * 		42   : CMP0
 * 		45   : DAC0
 * 		49-53: Port Control (Ports A-E)
 * 		54-56: TPM0-2 (Overflow)
 * 		57   : TSI
 * 		60-63: DMAMUX (Always enabled)
 */
#define CHCFG_SOURCE\
	(54)

/**
 * \def		DSR_BCR_DONE
 * \brief	Transactions Done flag
 * \detail
 * 		Clear this flag when beginning DMA
 */
#define DSR_BCR_DONE\
	(1)

/**
 * \def		CHCFG_ENBL
 * \brief	Enable flag
 * \detail
 * 		Set this flag to start DMA
 */
#define CHCFG_ENBL\
	(1)

/**
 * \var		dma_source
 * \brief	Pointer to beginning of source data on reload
 */
uint16_t *dma_source;

/**
 * \var		dma_count
 * \brief	Number of bytes to transfer on reload
 */
uint32_t dma_count;

void init_onboard_dma(void)
{
	/**
     * Enable clock to DMA
     * Enable clock to DMA MUX
     */
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
    SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	/**
     * Disable DMA0 to allow for configuration
     */
    DMAMUX0->CHCFG[0] = 0;

	/**
     * Configure DMA0:
     * 	- Generate DMA interrupt when done
     * 	- Increment source
     * 	- Transfer words (16 bits)
     * 	- Enable peripheral request
     */
    DMA0->DMA[0].DCR =
    	DMA_DCR_EINT(DCR_EINT) |
		DMA_DCR_ERQ(DCR_ERQ) |
		DMA_DCR_CS(DCR_CS) |
		DMA_DCR_SINC(DCR_SINC) |
		DMA_DCR_SSIZE(DCR_SSIZE) |
		//DMA_DCR_DINC(DCR_DINC) |
		DMA_DCR_DSIZE(DCR_DSIZE);

	/**
     * Configure DMA0 IRQ
     * 	- Set the interrupt priority (range 0 to 3, with 0 being highest priority)
     * 	- Clear pending IRQ
     * 	- Enable IRQ
     */
	NVIC_SetPriority(DMA0_IRQn, 2);
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	NVIC_EnableIRQ(DMA0_IRQn);

	/**
     * Configure DMA0:
     * 	- To use TPM0 overflow as trigger
     */
	DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_SOURCE(CHCFG_SOURCE);
}

void start_onboard_dma(uint16_t *source, uint32_t count)
{
	/**
     * Initialize:
     * 	- Source pointer to DAC buffer
     * 	- Destination pointer to DAC0
     * 	- Byte count to number of bytes
     */
	DMA0->DMA[0].SAR = DMA_SAR_SAR((uint32_t)(source));
	DMA0->DMA[0].DAR = DMA_DAR_DAR((uint32_t)(&(DAC0->DAT[0])));
	//DMA0->DMA[0].DAR = DMA_DAR_DAR((uint32_t)(dac_buffer_test));
	DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(count);

	/**
	 * Clear the Done flag
	 */
	DMA0->DMA[0].DSR_BCR &= ~DMA_DSR_BCR_DONE(DSR_BCR_DONE);

	/**
	 * Set the Enable flag to begin DMA transfer
	 */
	DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL(CHCFG_ENBL);
}

void DMA0_IRQHandler(void)
{
	/**
	 * Set the Done flag
	 */
	DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE(DSR_BCR_DONE);

    /**
     * Begin DMA transfer
     */
    start_onboard_dma(dac_buffer, dac_buffer_samples << 1);
}
