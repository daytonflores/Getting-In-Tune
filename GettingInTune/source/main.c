/*
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    MKL25Z4_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "sine.h"
#include "systick.h"
#include "test_sine.h"

/**
 * \def		BUF_ADC_SIZE
 * \brief	Size of audio out buffer
 */
#define BUF_ADC_SIZE\
	(1024)

/**
 * \def		SAMPLE_COUNT_ADC
 * \brief	The number of samples to transfer at a time
 */
#define SAMPLE_COUNT_ADC\
	(100)

/**
 * \var		buf_adc
 * \brief	Buffer to hold audio out data
 */
uint8_t buf_adc[BUF_ADC_SIZE];

/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    /**
     * Initialize on-board DAC
     */
    init_onboard_dac();

    /**
     * Initialize on-board ADC
     */
    init_onboard_adc();

    /**
     * Initialize on-board DMA
     */
    init_onboard_dma(buf_adc, SAMPLE_COUNT_ADC);

    /**
     * Initialize SysTick on-board timer
     */
    init_onboard_systick();

    /**
     * Main infinite loop
     */
    while(1) {

        /**
         * Set by SysTick_Handler every TICK_SEC
         */
        if(tick){

            /**
             * Reset flag that was set by SysTick ISR
             */
        	tick = false;

            /**
             * Increment for timestamp purposes
             */
        	ticks_since_startup++;

            /**
             * Increment for timing purposes
             */
        	ticks_since_last_note++;
        }
    }
    return 0 ;
}