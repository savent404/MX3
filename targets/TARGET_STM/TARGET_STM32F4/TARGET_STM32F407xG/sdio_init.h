#pragma once

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

void __SDIO_Init(void);

#ifdef __cplusplus
}
#endif

extern SD_HandleTypeDef hsd;
extern DMA_HandleTypeDef hdma_sdio_rx;
extern DMA_HandleTypeDef hdma_sdio_tx;
