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
 * \var		extern uint8_t tpm_sc_ps;
 * \brief	Defined in tpm.c
 */
extern uint8_t tpm_sc_ps;

/**
 * \fn		void init_onboard_tpm
 * \param	N/A
 * \return	N/A
 * \brief   Initialize the on-board timer PWM
 */
void init_onboard_tpm(void);

/**
 * \fn		uint8_t get_prescaler
 * \param	N/A
 * \return	x for 2^x, where 2^x is the TPM prescaler
 * \brief   Calculate the smallest necessary TPM prescaler to provide the highest granularity
 */
uint8_t get_prescaler(void);

#endif /* TPM_H_ */
