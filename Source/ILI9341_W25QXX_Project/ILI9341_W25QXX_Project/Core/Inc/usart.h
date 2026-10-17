/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */



/* USER CODE END Includes */

extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */

#define  g_uart2_handle huart2

/**
 * @brief uart max rx buffer length definition
 */
#define UART_MAX_LEN       512        /**< uart2 max len */



typedef struct __UART_Buffer
{
uint8_t g_uart_rx_buffer[UART_MAX_LEN];        /**< uart2 rx buffer */
uint8_t g_uart_buffer;                          /**< uart2 one buffer */
volatile uint16_t g_uart_point;                 /**< uart2 rx point */
volatile uint8_t g_uart_tx_done;                /**< uart2 tx done flag */
} UART_BufferTypeDef;



/* USER CODE END Private defines */

void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle);
void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle);


/**
 * @brief  uart2 deint
 * @return status code
 *         - 0 success
 *         - 1 deinit
 * @note   none
 */
uint8_t uart2_deinit(void);

/**
 * @brief     uart2 write data
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t uart2_write(uint8_t *buf, uint16_t len);

/**
 * @brief      uart2 read data
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len data length
 * @return     length of the read data
 * @note       this function will clear all received buffer even read length is less than received length
 */
uint16_t uart_read(uint8_t *buf, uint16_t len, UART_HandleTypeDef* handle);

/**
 * @brief  uart2 flush data
 * @return status code
 *         - 0 success
 * @note   none
 */
uint16_t uart2_flush(void);

/**
 * @brief  uart2 get the handle
 * @return pointer to a uart handle
 * @note   none
 */

uint16_t uart2_print(const char *const fmt, ...);


UART_HandleTypeDef* uart2_get_handle(void);

/**
 * @brief uart2 set tx done
 * @note  none
 */
void uart2_set_tx_done(void);

/**
 * @brief uart2 irq handler
 * @note  none
 */
void uart2_irq_handler(void);

/**
 * @}
 */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

