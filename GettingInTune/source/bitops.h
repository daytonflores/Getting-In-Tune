/**
 * \file    bitops.h
 * \author	Dayton Flores (dafl2542@colorado.edu)
 * \date	10/14/2022
 * \brief   Macros and function headers for bit operations
 */

#ifndef BITOPS_H_
#define BITOPS_H_

/**
 * \def		MASK(x)
 * \param x	The amount of bits 1 should be shifted to the left
 * \brief	Referenced from Chapter 2 of Alexander G Dean's Embedded Systems Fundamentals with
 * 			ARM Cortex-M Based Microcontrollers: Embedded Systems
 */
#define MASK(x)\
	((uint32_t)(1UL << (x)))

#endif /* BITOPS_H_ */
