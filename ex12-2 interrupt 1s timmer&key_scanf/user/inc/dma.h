#ifndef _DMA_H_
#define _DMA_H_
#include "stm32f4xx.h"

void dma_init(u32 par);

void dma2_stream2_tranf(u32 mar,u16 ndtr);
#endif


