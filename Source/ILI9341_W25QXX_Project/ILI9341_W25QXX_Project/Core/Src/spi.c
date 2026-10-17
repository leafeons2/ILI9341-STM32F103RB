/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SPI_HandleTypeDef hspi1;

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = DIS_W25_SCK_Pin|DIS_W25_MOSI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = DIS_W25_MISO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DIS_W25_MISO_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOA, DIS_W25_SCK_Pin|DIS_W25_MISO_Pin|DIS_W25_MOSI_Pin);

  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/*
 * 직접 야매로 제작한 SPI Deinit 함수
 * 주로 ILI9341 사용을 마친 후 사용
 */

uint8_t SPI1_DeInit(void)
{
    if (HAL_SPI_DeInit(&hspi1) != HAL_OK)
    {
        return 1;
    }
    return 0;
}



/**
 * @brief     spi bus write command
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t spi_write_cmd(uint8_t *buf, uint16_t len)
{
    uint8_t res;

    /* set cs low */
    HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_RESET);

    /* if len > 0 */
    if (len > 0)
    {
        /* transmit the buffer */
        res = HAL_SPI_Transmit(&hspi1, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

            return 1;
        }
    }

    /* set cs high */
    HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin , GPIO_PIN_SET);

    return 0;
}

/**
 * @brief     spi bus write
 * @param[in] addr spi register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t w25qxx_spi_write(uint8_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t buffer;
    uint8_t res;

    /* set cs low */
    HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_RESET);

    /* transmit the addr */
    buffer = addr;
    res = HAL_SPI_Transmit(&hspi1, (uint8_t *)&buffer, 1, 1000);
    if (res != HAL_OK)
    {
        /* set cs high */
        HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

        return 1;
    }

    /* if len > 0 */
    if (len > 0)
    {
        /* transmit the buffer */
        res = HAL_SPI_Transmit(&hspi1, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

            return 1;
        }
    }

    /* set cs high */
    HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

    return 0;
}


/**
 * @brief     spi bus write address 16
 * @param[in] addr spi register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t spi_write_address16(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t buffer[2];
    uint8_t res;

    /* set cs low */
    HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_RESET);

    /* transmit the addr  */
    buffer[0] = (addr >> 8) & 0xFF;
    buffer[1] = addr & 0xFF;
    res = HAL_SPI_Transmit(&hspi1, (uint8_t *)buffer, 2, 1000);
    if (res != HAL_OK)
    {
        /* set cs high */
        HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

        return 1;
    }

    /* if len > 0 */
    if (len > 0)
    {
        /* transmit the buffer */
        res = HAL_SPI_Transmit(&hspi1, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

            return 1;
        }
    }

    /* set cs high */
    HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

    return 0;
}

/**
 * @brief      spi bus read command
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t spi_read_cmd(uint8_t *buf, uint16_t len)
{
    uint8_t res;

    /* set cs low */
    HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_RESET);

    /* if len > 0 */
    if (len > 0)
    {
        /* receive to the buffer */
        res = HAL_SPI_Receive(&hspi1, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

            return 1;
        }
    }

    /* set cs high */
    HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

    return 0;
}

/**
 * @brief      spi bus read
 * @param[in]  addr spi register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t spi_read(uint8_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t buffer;
    uint8_t res;

    /* set cs low */
    HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_RESET);

    /* transmit the addr */
    buffer = addr;
    res = HAL_SPI_Transmit(&hspi1, (uint8_t *)&buffer, 1, 1000);
    if (res != HAL_OK)
    {
        /* set cs high */
        HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

        return 1;
    }

    /* if len > 0 */
    if (len > 0)
    {
        /* receive to the buffer */
        res = HAL_SPI_Receive(&hspi1, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

            return 1;
        }
    }

    /* set cs high */
    HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

    return 0;
}

/**
 * @brief      spi bus read address 16
 * @param[in]  addr spi register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t spi_read_address16(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t buffer[2];
    uint8_t res;

    /* set cs low */
    HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_RESET);

    /* transmit the addr  */
    buffer[0] = (addr >> 8) & 0xFF;
    buffer[1] = addr & 0xFF;
    res = HAL_SPI_Transmit(&hspi1, (uint8_t *)buffer, 2, 1000);
    if (res != HAL_OK)
    {
        /* set cs high */
        HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

        return 1;
    }

    /* if len > 0 */
    if (len > 0)
    {
        /* receive to the buffer */
        res = HAL_SPI_Receive(&hspi1, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

            return 1;
        }
    }

    /* set cs high */
    HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

    return 0;
}

/**
 * @brief      spi transmit
 * @param[in]  *tx pointer to a tx buffer
 * @param[out] *rx pointer to a rx buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 transmit failed
 * @note       none
 */
uint8_t spi_transmit(uint8_t *tx, uint8_t *rx, uint16_t len)
{
    uint8_t res;

    /* set cs low */
    HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_RESET);

    /* if len > 0 */
    if (len > 0)
    {
        /* transmit */
        res = HAL_SPI_TransmitReceive(&hspi1, tx, rx, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

            return 1;
        }
    }

    /* set cs high */
    HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

    return 0;
}

/**
 * @brief      spi bus write read
 * @param[in]  *in_buf pointer to an input buffer
 * @param[in]  in_len input length
 * @param[out] *out_buf pointer to an output buffer
 * @param[in]  out_len output length
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */
uint8_t spi_write_read(uint8_t *in_buf, uint32_t in_len, uint8_t *out_buf, uint32_t out_len)
{
    uint8_t res;

    /* set cs low */
    HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_RESET);

    /* if in_len > 0 */
    if (in_len > 0)
    {
        /* transmit the input buffer */
        res = HAL_SPI_Transmit(&hspi1, in_buf, in_len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

            return 1;
        }
    }

    /* if out_len > 0 */
    if (out_len > 0)
    {
        /* transmit to the output buffer */
        res = HAL_SPI_Receive(&hspi1, out_buf, out_len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

            return 1;
        }
    }

    /* set cs high */
    HAL_GPIO_WritePin(W25_CS_GPIO_Port, W25_CS_Pin, GPIO_PIN_SET);

    return 0;
}


/* USER CODE END 1 */

