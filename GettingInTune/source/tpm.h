/**
 * \file    tpm.h
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/16/2022
 * \brief   Macros and function headers for TPM (Timer PWM Module)
 */

#ifndef TPM_H_
#define TPM_H_

/**
 * \def		PWM_FREQ_HZ
 * \brief	The desired frequency of the PWM in Hz
 */
#define PWM_FREQ_HZ\
	(500)

/**
 * \var		extern uint8_t tpm_sc_ps;
 * \brief	Defined in tpm.c
 */
extern uint8_t tpm_sc_ps;

/**
 * \fn		void init_onboard_tpm
 * \param	uint32_t dac_period_us
 * \param	uint32_t adc_period_us
 * \return	N/A
 * \brief   Initialize the on-board timer PWM
 */
void init_onboard_tpm(uint32_t dac_period_us, uint32_t adc_period_us);

/**
 * \fn		void start_onboard_tpm
 * \param	N/A
 * \return	N/A
 * \brief   Start the on-board TPM
 */
void start_onboard_tpm(void);

/**
 * \fn		void TPM1_IRQHandler
 * \param	N/A
 * \return	N/A
 * \brief   The ISR for the TPM1 timer (i.e. runs each time the timer overflows).
 * \detail	FUNCTION NAME IS CASE SENSITIVE. Since it is weakly defined in
 * 			startup\startup_mkl25z4.c this definition will override
 */
void TPM1_IRQHandler(void);

#endif /* TPM_H_ */
