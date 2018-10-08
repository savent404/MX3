#pragma once

#include "cmsis.h"

void __SPI_Init(void);
extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_tx;
