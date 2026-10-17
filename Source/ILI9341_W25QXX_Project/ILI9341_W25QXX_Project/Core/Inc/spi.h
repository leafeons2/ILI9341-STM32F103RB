/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.h
  * @brief   This file contains all the function prototypes for
  *          the spi.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN Private defines */

/**
 * @defgroup spi spi function
 * @brief    spi function modules
 * @{
 */

/**
 * @brief spi mode enumeration definition
 */
typedef enum
{
    SPI_MODE_0 = 0x00,        /**< mode 0 */
    SPI_MODE_1 = 0x01,        /**< mode 1 */
    SPI_MODE_2 = 0x02,        /**< mode 2 */
    SPI_MODE_3 = 0x03,        /**< mode 3 */
} spi_mode_t;



/* USER CODE END Private defines */

void MX_SPI1_Init(void);

/* USER CODE BEGIN Prototypes */

uint8_t SPI1_DeInit(void);



/**
 * @brief     spi bus write command
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t spi_write_cmd(uint8_t *buf, uint16_t len);

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
uint8_t w25qxx_spi_write(uint8_t addr, uint8_t *buf, uint16_t len);

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
uint8_t spi_write_address16(uint16_t addr, uint8_t *buf, uint16_t len);

/**
 * @brief      spi bus read command
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t spi_read_cmd(uint8_t *buf, uint16_t len);

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
uint8_t spi_read(uint8_t addr, uint8_t *buf, uint16_t len);

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
uint8_t spi_read_address16(uint16_t addr, uint8_t *buf, uint16_t len);

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
uint8_t spi_write_read(uint8_t *in_buf, uint32_t in_len, uint8_t *out_buf, uint32_t out_len);

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
uint8_t spi_transmit(uint8_t *tx, uint8_t *rx, uint16_t len);

/**
 * @}
 */


/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

