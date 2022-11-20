/*
 * dma.h
 *
 *  Created on: Nov 17, 2022
 *      Author: dayton.flores
 */

#ifndef DMA_H_
#define DMA_H_

/**
 * \fn		void init_onboard_dma
 * \param	uint16_t *source The beginning of the source data to transfer
 * \param	uint32_t sample_count The number of samples to transfer at a time
 * \return	N/A
 * \brief   Initialize the on-board DMA
 */
void init_onboard_dma(uint16_t *source, uint32_t sample_count);

#endif /* DMA_H_ */
