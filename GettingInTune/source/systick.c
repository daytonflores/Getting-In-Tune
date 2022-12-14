/**
 * \file    systick.c
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/16/2022
 * \brief   Function definitions for on-board SysTick
 */

#include <stdbool.h>
#include <stdint.h>
#include <systick.h>
#include "board.h"
#include "core_cm0plus.h"
#include "fsl_debug_console.h"

/**
 * User-defined libraries
 */
#include "bitops.h"
#include "systick.h"

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
 * \var		ticktime_t ticks_since_startup
 * \brief	Ticks since boot
 */
volatile ticktime_t ticks_since_startup = 0;

/**
 * \var		ticktime_t ticks_since_last_note
 * \brief	Ticks since the last note was played
 */
volatile ticktime_t ticks_since_last_note = 0;

/**
 * \var		volatile bool tick
 * \brief	Flag controlled by SysTick timer
 */
volatile bool tick = false;

void init_onboard_systick(void)
{
    /**
     * Configure the SysTick LOAD register:
     * 	- To generate interrupt every TICK_SEC
     */
	SysTick->LOAD = ((ALT_CLOCK_HZ - 1) * TICK_SEC);

	/**
     * Set the SysTick interrupt priority (range 0 to 3, with 0 being highest priority)
     */
	NVIC_SetPriority(SysTick_IRQn, 3);

	/**
     * Configure SysTick VAL register:
     * 	- Reset Value register since this is where the current SysTick timer value will be read from
     */
	SysTick->VAL = 0;

	/**
     * Configure SysTick CTRL register:
     * 	- Operate using processor reference clock
     * 	- Enable SysTick exception register
     */
	SysTick->CTRL =
		SysTick_CTRL_CLKSOURCE_EXT_Msk |
		SysTick_CTRL_TICKINT_Msk;

    /**
     * Modify SysTick->CTRL register to enable the counter
     */
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
    /**
     * Raise flag that TICK_SEC time has passed
     */
	tick = true;
}

volatile uint32_t now(void)
{
    /**
     * Convert ticks to sec, and then sec to ms
     */
	return((ticks_since_startup * TICK_SEC * MSEC_PER_SEC));
}
