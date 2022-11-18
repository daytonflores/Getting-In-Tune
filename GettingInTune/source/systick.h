/**
 * \file    systick.h
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/16/2022
 * \brief   Macros and function headers for on-board timing subsystem
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

/**
 * \def		MSEC_PER_SEC
 * \brief	Used for unit conversions
 */
#define MSEC_PER_SEC\
	(1000)

/**
 * \def		PRIM_CLOCK_HZ
 * \brief	Frequency of primary processor clock source in Hz. For KL25Z, this is 48 MHz
 */
#define PRIM_CLOCK_HZ\
	(48000000UL)

/**
 * \def		ALT_CLOCK_HZ
 * \brief	Frequency of alternate clock source in Hz. For KL25Z, this is 3 MHz
 */
#define ALT_CLOCK_HZ\
	(3000000UL)

/**
 * \def		SysTick_CTRL_CLKSOURCE_EXT_Msk
 * \brief	The CLKSOURCE field selects the clock source, which can be either the processor
 * 			clock (1) or an external reference clock (0). On the KL25Z MCU, the processor
 * 			clock runs at up to 48 MHz, and the external reference clock is the processor clock
 * 			divided by 16
 */
#define SysTick_CTRL_CLKSOURCE_EXT_Msk\
	(0UL << SysTick_CTRL_CLKSOURCE_Pos)

/**
 * \def		TICK_HZ
 * \brief	The frequency at which SysTick interrupts should be raised in Hz
 */
#define TICK_HZ\
	(16)

/**
 * \def		TICK_SEC
 * \brief	The period between SysTick interrupts in sec
 */
#define TICK_SEC\
	(1.0/TICK_HZ)

/**
 * \typedef	typedef unsigned int ticktime_t
 * \brief   Time since boot, where each tick is 1 / ALT_CLOCK_HZ or 1 / PRIM_CLOCK_HZ, depending
 * 			on what's configured
 */
typedef unsigned int ticktime_t;

/**
 * \var		ticktime_t ticks_since_startup
 * \brief	Defined in systick.c
 */
extern volatile ticktime_t ticks_since_startup;

/**
 * \var		ticktime_t ticks_since_last_note
 * \brief	Defined in systick.c
 */
extern volatile ticktime_t ticks_since_last_note;

/**
 * \var		extern volatile bool tick
 * \brief	Defined in systick.c
 */
extern volatile bool tick;

/**
 * \fn		void init_onboard_systick
 * \param	N/A
 * \return	N/A
 * \brief   Initialize the timing system
 */
void init_onboard_systick(void);

/**
 * \fn		void SysTick_Handler
 * \param	N/A
 * \return	N/A
 * \brief   The ISR for the SysTick timer (i.e. runs each time the timer reaches 0).
 * \detail	FUNCTION NAME IS CASE SENSITIVE. Since it is weakly defined in
 * 			startup\startup_mkl25z4.c this definition will override
 */
void SysTick_Handler(void);

/**
 * \fn		uint32_t now
 * \param	N/A
 * \return	Time since startup in ms
 * \brief   Returns time since startup, in ms
 */
volatile uint32_t now(void);

/**
 * \fn		void reset_timer
 * \param	N/A
 * \return	N/A
 * \brief   Resets timer to 0; doesn't affect now() values
 */
void reset_timer(void);

/**
 * \fn		ticktime_t get_timer
 * \param	N/A
 * \return	Ticks since the last call to reset_timer()
 * \brief   Returns ticks since the last call to reset_timer()
 */
ticktime_t get_timer(void);


#endif /* SYSTICK_H_ */
