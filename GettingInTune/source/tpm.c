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
 * \var		uint8_t tpm_sc_ps;
 * \brief	Holds x for 2^x, where 2^x is the tpm_prescaler
 */
uint8_t tpm_sc_ps;

void init_onboard_tpm(void)
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
     * Set the smallest needed prescaler along with PWM period for desired PWM frequency
     */
	tpm_sc_ps = get_prescaler();

	/**
     * Load the TPM MOD register
     */
	TPM0->MOD = 1;

	/**
     * Configure the TPM SC register:
     * 	- Count up with divide by tpm_prescaler
     */
	TPM0->SC = TPM_SC_PS(tpm_sc_ps);

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

	/**
     * Configure the TPM SC register:
     * 	- Start TPM
     */
	TPM0->SC |= TPM_SC_CMOD(1);
}

uint8_t get_prescaler(void)
{
	/**
     * Calculate the smallest needed prescaler to allow for largest possible TPM->MOD value
     * and thus more granular control
     */

	uint8_t return_value;

	if(((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / MAX_TPM_MOD_VALUE) < 1){
		return_value = 0;
	}
	else if(((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / MAX_TPM_MOD_VALUE) < 2){
		return_value = 1;
	}
	else if(((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / MAX_TPM_MOD_VALUE) < 4){
		return_value = 2;
	}
	else if(((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / MAX_TPM_MOD_VALUE) < 8){
		return_value = 3;
	}
	else if(((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / MAX_TPM_MOD_VALUE) < 16){
		return_value = 4;
	}
	else if(((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / MAX_TPM_MOD_VALUE) < 32){
		return_value = 5;
	}
	else if(((F_TPM_CLOCK_HZ / PWM_FREQ_HZ) / MAX_TPM_MOD_VALUE) < 64){
		return_value = 6;
	}
	else{
		return_value = 7;
	}

	return (return_value);
}
