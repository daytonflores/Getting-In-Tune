/**
 * \file    tpm.c
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/16/2022
 * \brief   Function definitions for TPM (Timer PWM Module)
 */

#include "board.h"
#include "fsl_debug_console.h"

/**
 * User-defined libraries
 */
#include "bitops.h"
#include "tpm.h"

/**
 * \def		MAX_TPM_MOD_VALUE
 * \brief	The max division factor for 16-bit TPM->MOD register
 */
#define MAX_TPM_MOD_VALUE\
	(65536)

/**
 * \def		TPM_CLOCK_SRC
 * \brief	Configuration for TPM clock source select
 * \detail
 * 		0: Disabled
 * 		1: MCGFLLCLK (or MCGPLLCLK / 2)
 * 		2: OSCERCLK
 * 		3: MCGIRCLK
 */
#define TPM_CLOCK_SRC\
	(1)

/**
 * \def		TPM_DBGMODE
 * \brief	Configuration for TPM debug mode
 * \detail
 * 		0: LPTPM counter does not increment during debug. Trigger inputs and input capture events
 * 		   are also ignored
 * 		3: LPTPM counter continues to increment in debug mode
 */
#define TPM_DBGMODE\
	(3)

/**
 * \def		F_TPM_CLOCK_HZ
 * \brief	The frequency of TPM clock in Hz
 */
#define F_TPM_CLOCK_HZ\
	(48000000)

/**
 * \def		SC_DMA
 * \brief	DMA Enable
 * \detail
 * 		0: Disables DMA transfers
 * 		1: Enables DMA transfers for the overflow flag
 */
#define SC_DMA\
	(1)

/**
 * \def		SC_PS
 * \brief	Prescale Factor Selection
 * \detail
 * 		000: Divide by 1
 * 		001: Divide by 2
 * 		010: Divide by 4
 * 		011: Divide by 8
 * 		100: Divide by 16
 * 		101: Divide by 32
 * 		110: Divide by 64
 * 		111: Divide by 128
 */
#define SC_PS\
	(1)

/**
 * \var		uint8_t tpm_sc_ps;
 * \brief	Holds x for 2^x, where 2^x is the tpm_prescaler
 */
uint8_t tpm_sc_ps;

void init_onboard_tpm(uint32_t period_us)
{
	/**
	 * Enable clock to TPM module
	 */
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

	/**
     * Configure SOPT2:
     * 	- To use MCGFLLCLK and MCGPLLCLK/2 as clock source
     * 	- To select FLL clock from above
     */
	SIM->SOPT2 |=
		SIM_SOPT2_TPMSRC(TPM_CLOCK_SRC) |
		SIM_SOPT2_PLLFLLSEL_MASK;

	/**
	 * Disable TPM for configuration
	 */
	TPM0->SC = 0;

	/**
     * Set the smallest needed prescaler for desired TPM period
     */
	//tpm_sc_ps = get_prescaler(period_us);

	/**
     * Load the TPM MOD register
     */
	TPM0->MOD = TPM_MOD_MOD(period_us * 24);

	/**
     * Configure the TPM SC register:
     * 	- Count up with divide by 2 prescaler
     * 	- DMA transfer enable
     */
	TPM0->SC =
		TPM_SC_DMA(SC_DMA) |
		TPM_SC_PS(SC_PS);

	/**
     * Configure the TPM CONF register:
     * 	- Continue counting operation in debug mode
     */
	TPM0->CONF |= TPM_CONF_DBGMODE(TPM_DBGMODE);

	/**
     * Configure TPM CnSC:
     * 	- Edge-aligned PWM
     * 	- Low-true pulses (set output on match, clear output on reload)
     */
	TPM0->CONTROLS[1].CnSC =
		TPM_CnSC_MSB_MASK |
		TPM_CnSC_ELSA_MASK;


	/**
     * Set initial duty cycle
     */
	TPM0->CONTROLS[1].CnV = 0;
}

void start_onboard_tpm(void)
{
	/**
     * Configure the TPM SC register:
     * 	- Start TPM
     */
	TPM0->SC |= TPM_SC_CMOD(1);
}
