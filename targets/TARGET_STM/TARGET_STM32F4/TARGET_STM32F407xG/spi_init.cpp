#include "spi_init.h"

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi3;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi3_tx;

static void MX_SPI_Init(void);
static void MX_DMA_Init(void);
static void MX_GPIO_Init(void);

/**
* @brief This function handles DMA1 stream7 global interrupt.
*/
void DMA1_Stream7_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_spi3_tx);
}

/**
* @brief This function handles DMA2 stream0 global interrupt.
*/
void DMA2_Stream0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_spi1_rx);
}

void __SPI_Init(void)
{
    MX_GPIO_Init();
    MX_SPI_Init();
    MX_DMA_Init();
}

/* SPI1 init function */
static void MX_SPI_Init(void)
{
    /* SPI1 parameter configuration*/
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    HAL_SPI_Init(&hspi1);

    /* SPI3 parameter configuration*/
    hspi3.Instance = SPI3;
    hspi3.Init.Mode = SPI_MODE_MASTER;
    hspi3.Init.Direction = SPI_DIRECTION_1LINE;
    hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi3.Init.NSS = SPI_NSS_SOFT;
    hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi3.Init.CRCPolynomial = 10;
    HAL_SPI_Init(&hspi3);
}
/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Stream7_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);
    /* DMA2_Stream0_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

    NVIC_SetVector(DMA1_Stream7_IRQn, (uint32_t)DMA1_Stream7_IRQHandler);
    NVIC_SetVector(DMA2_Stream0_IRQn, (uint32_t)DMA2_Stream0_IRQHandler);
}
/** Pinout Configuration
*/
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if(hspi->Instance==SPI1)
    {
      /* Peripheral clock enable */
      __HAL_RCC_SPI1_CLK_ENABLE();

      /**SPI1 GPIO Configuration
      PA5     ------> SPI1_SCK
      PA6     ------> SPI1_MISO
      PA7     ------> SPI1_MOSI
      */
      GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

      /* SPI1 DMA Init */
      /* SPI1_RX Init */
      hdma_spi1_rx.Instance = DMA2_Stream0;
      hdma_spi1_rx.Init.Channel = DMA_CHANNEL_3;
      hdma_spi1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
      hdma_spi1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
      hdma_spi1_rx.Init.MemInc = DMA_MINC_ENABLE;
      hdma_spi1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      hdma_spi1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
      hdma_spi1_rx.Init.Mode = DMA_NORMAL;
      hdma_spi1_rx.Init.Priority = DMA_PRIORITY_LOW;
      hdma_spi1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
      HAL_DMA_Init(&hdma_spi1_rx);

      __HAL_LINKDMA(hspi,hdmarx,hdma_spi1_rx);
    }
    else if(hspi->Instance==SPI3)
    {
      /* Peripheral clock enable */
      __HAL_RCC_SPI3_CLK_ENABLE();

      /**SPI3 GPIO Configuration
      PC10     ------> SPI3_SCK
      PC12     ------> SPI3_MOSI
      */
      GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

      /* SPI3 DMA Init */
      /* SPI3_TX Init */
      hdma_spi3_tx.Instance = DMA1_Stream7;
      hdma_spi3_tx.Init.Channel = DMA_CHANNEL_0;
      hdma_spi3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
      hdma_spi3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
      hdma_spi3_tx.Init.MemInc = DMA_MINC_ENABLE;
      hdma_spi3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      hdma_spi3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
      hdma_spi3_tx.Init.Mode = DMA_NORMAL;
      hdma_spi3_tx.Init.Priority = DMA_PRIORITY_LOW;
      hdma_spi3_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
      HAL_DMA_Init(&hdma_spi3_tx);
      __HAL_LINKDMA(hspi,hdmatx,hdma_spi3_tx);
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
    if(hspi->Instance==SPI1)
    {
      /* Peripheral clock disable */
      __HAL_RCC_SPI1_CLK_DISABLE();

      /**SPI1 GPIO Configuration
      PA5     ------> SPI1_SCK
      PA6     ------> SPI1_MISO
      PA7     ------> SPI1_MOSI
      */
      HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

      /* SPI1 DMA DeInit */
      HAL_DMA_DeInit(hspi->hdmarx);
    }
    else if(hspi->Instance==SPI3)
    {
      /* Peripheral clock disable */
      __HAL_RCC_SPI3_CLK_DISABLE();

      /**SPI3 GPIO Configuration
      PC10     ------> SPI3_SCK
      PC12     ------> SPI3_MOSI
      */
      HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_12);

      /* SPI3 DMA DeInit */
      HAL_DMA_DeInit(hspi->hdmatx);
    }
}
