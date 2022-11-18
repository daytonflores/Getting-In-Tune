/*
 * adc.c
 *
 *  Created on: Nov 17, 2022
 *      Author: dayton.flores
 */

#include "board.h"
#include "adc.h"

/**
 * \def		PCR_MUX_SEL_ADC
 * \brief	PCR is a 32-bit register where bits 8:10 are a MUX field
 * \detail
 * 		000: Pin disabled (analog)
 * 		001: GPIO
 * 		010: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		011: TPM2_CH0
 * 		100: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		101: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		110: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 * 		111: Alternative (see Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			 ARM Cortex-M Based Microcontrollers: Embedded Systems)
 */
#define PCR_MUX_SEL_ADC\
	(0)

/**
 * \def		PORTE_ADC0_POS
 * \brief	ADC0 Analog output is located at PTE20
 */
#define PORTE_ADC0_POS\
	(20)

/**
 * \def		CFG1_ADLPC
 * \brief	CFG1[7] which is Low-Power Configuration
 * \detail
 * 		0: Normal power configuration
 * 		1: Low-power configuration. The power is reduced
 * 		   at the expense of maximum clock speed
 */
#define CFG1_ADLPC\
	(1)

/**
 * \def		CFG1_ADLSMP
 * \brief	CFG1[4] which is Sample Time Configuration
 * \detail
 * 		0: Short sample time
 * 		1: Long sample time allows higher impedance inputs to be
 * 		   accurately sampled or to maximize conversion speed for
 * 		   lower impedance inputs. Longer sample times can also be
 * 		   used to lower overall power consumption if continuous
 * 		   conversions are enabled and high conversion rates are
 * 		   not required. When ADLSMP=1, the long sample time
 * 		   select bits, (ADLSTS[1:0]), can select the extent of
 * 		   the long sample time.
 */
#define CFG1_ADLSMP\
	(1)

/**
 * \def		CFG1_MODE
 * \brief	CFG1[3:2] which is Conversion Mode
 * \detail
 * 		00: When DIFF=0, it is single-ended 8-bit conversion.
 * 		    When DIFF=1, it is differential 9-bit conversion with
 * 		    2's complement output
 * 		01: When DIFF=0, it is single-ended 12-bit conversion.
 * 		    When DIFF=1, it is differential 13-bit conversion with
 * 		    2's complement output
 * 		10: When DIFF=0, it is single-ended 10-bit conversion.
 * 		    When DIFF=1, it is differential 11-bit conversion with
 * 		    2's complement output
 * 		11: When DIFF=0, it is single-ended 16-bit conversion.
 * 		    When DIFF=1, it is differential 16-bit conversion with
 * 		    2's complement output
 */
#define CFG1_MODE\
	(3)

/**
 * \def		CFG1_ADICLK
 * \brief	CFG1[1:0] which is Input Clock Select
 * \detail
 * 		00: Bus Clock
 * 		01: (Bus Clock) / 2
 * 		10: Alternate Clock (ALTCLK)
 * 		11: Asynchronous Clock (ADACK). It is not required to be active
 * 		    prior to conversion start. When it is selected and it is not
 * 		    active prior to a conversion start, when CFG2[ADACKEN]=0,
 * 		    the asynchronous clock is activated at the start of a conversion
 * 		    and deactivated when conversions are terminated. In this case,
 * 		    there is an associated clock startup delay each time the clock
 * 		    source is re-activated
 */
#define CFG1_ADICLK\
	(0)

/**
 * \def		SC2_REFSEL
 * \brief	SC2[1:0] which is Voltage Reference Select
 * \detail
 * 		00: Default voltage reference pin pair, that is,
 * 		    external pins VREFH and VREFL
 * 		01: Alternate reference pair, that is, VALTH and VALTL.
 * 		    This pair may be additional external pins or
 * 		    internal sources depending on the MCU configuration.
 * 		    See the chip configuration information for details
 * 		    specific to this MCU
 * 		10: Reserved
 * 		11: Reserved
 */
#define SC2_REFSEL\
	(0)

void init_onboard_adc(void)
{
	/**
     * Enable clock to Port E
     * Enable clock to DAC0
     */
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

    /**
     * Set PTE20 to Analog
     *
     * The MUX selection in PCR is done with bits 10:8, where 000 is configuration as Analog
     */
    PORTE->PCR[PORTE_ADC0_POS] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[PORTE_ADC0_POS] |= PORT_PCR_MUX(PCR_MUX_SEL_ADC);

	/**
	 * Configure ADC0:
	 * 	- Low power configuration
	 * 	- Long sample time
	 * 	- 16-bit single-ended conversion
	 * 	- Bus clock
	 */
    ADC0->CFG1 =
    	ADC_CFG1_ADLPC(CFG1_ADLPC) |
		ADC_CFG1_ADLSMP(CFG1_ADLSMP) |
		ADC_CFG1_MODE(CFG1_MODE) |
		ADC_CFG1_ADICLK(CFG1_ADICLK);

	/**
	 * Configure ADC0:
	 * 	- Software trigger
	 * 	- Compare function disabled
	 * 	- DMA disabled
	 * 	- Voltage references VREFH and VREFL
	 */
    ADC0->SC2 =
    	ADC_SC2_REFSEL(SC2_REFSEL);
}
