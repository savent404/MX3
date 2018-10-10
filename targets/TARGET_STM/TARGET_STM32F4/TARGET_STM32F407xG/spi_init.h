#pragma once

#include "cmsis.h"

void __SPI_Init(void);
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi3;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi3_tx;
