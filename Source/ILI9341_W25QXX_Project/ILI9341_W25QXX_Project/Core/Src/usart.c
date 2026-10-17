/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>



/**
 * @brief uart2 var definition
 */


UART_BufferTypeDef uart2_Buffer;


/* USER CODE END 0 */

UART_HandleTypeDef huart2;

/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */


  if (HAL_UART_Receive_IT(&huart2, (uint8_t *)&(uart2_Buffer.g_uart_buffer), 1) != HAL_OK)
  {
	  while(1)
	  {
	  }
  }

  /* USER CODE END USART2_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = PC_MCU_MCU_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(PC_MCU_MCU_TX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = PC_MCU_MCU_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(PC_MCU_MCU_RX_GPIO_Port, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, PC_MCU_MCU_TX_Pin|PC_MCU_MCU_RX_Pin);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


/**
 * @brief  uart deint
 * @return status code
 *         - 0 success
 *         - 1 deinit
 * @note   none
 */
uint8_t uart2_deinit(void)
{
    /* uart deinit */
    if (HAL_UART_DeInit(&g_uart2_handle) != HAL_OK)
    {
        return 1;
    }
    return 0;
}



/**
 * @brief     uart write data
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t uart2_write(uint8_t *buf, uint16_t len)
{
    uint16_t timeout = 1000;

    /* set tx done 0 */
    uart2_Buffer.g_uart_tx_done = 0;

    /* transmit */
    if (HAL_UART_Transmit_IT(&g_uart2_handle, (uint8_t *)buf, len) != HAL_OK)
    {
        return 1;
    }   /*
     	 *	인터럽트가 발동해서HAL_UART_TxCpltCallback 가 발동되면,
     	 *	uart2_Buffer.g_uart_tx_done = 1 만 수행된다.
     	 *
     	 */


    /* wait for events */
    while ((uart2_Buffer.g_uart_tx_done == 0) && (timeout != 0))
    {
        HAL_Delay(1);
        timeout--;
        /*
         *  수신과 송신이 동시에 진행될 가능성이 적긴하지만
         *  만약 진행되면 터미널에 보이는 문자열이 섞여 보이니까 Polling으로 검사한다
         * 	검사는 1초를 넘기지 않음
         */
    }

    /* check the timeout */
    if (timeout != 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }

    /*
     *  위 while 루프 안에 전송이 마무리 되길 기대하며 timeout에 값이 남아있을때만
     *  0의 값을 반환하는 것 같다.
     */

}



/**
 * @brief      uart read data
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len data length
 * @return     length of the read data
 * @note       this function will clear all received buffer even read length is less than received length
 */
uint16_t uart_read(uint8_t *buf, uint16_t len, UART_HandleTypeDef* handle)
{
	if(handle != &huart2)
	{
		return 1;
	}
    uint16_t read_len;
    uint16_t g_uart_point_old;

    uint8_t *CR_pointer;

    /* check receiving */

    while(1)
    {
    	g_uart_point_old = uart2_Buffer.g_uart_point;
    	HAL_Delay(5);
    	if (uart2_Buffer.g_uart_point == g_uart_point_old)
    	{
    		break;
    	}
    }
    /*
     *  1ms 동안 새로운 입력이 수신되었는지 감지하는 조건문
     *  1)현재 입력이 진행중인 상태였다
     *    : 이 상태로 uart2_read 에 들어가면, 입력이 다 안 끝난 값이 존재할 수 있다
     *      입력은 us 단위로 바뀌며, 1ms 동안 기다린다면, 불완전한 입력이 완료될 수 있다
     *  2)입력이 이미 완료된 상태이다
     *    :  딱히 변화 없다.  유저 기다림 상 1ms 는 충분히 짧아서 delay 가 체감되지 않는다.
     *
     */

    CR_pointer =  (uint8_t*)strstr((char*)(uart2_Buffer.g_uart_rx_buffer), "\r");
    if(CR_pointer != NULL)
    {
    	*CR_pointer = '\0';
    	uart2_Buffer.g_uart_point = strlen((char*)(uart2_Buffer.g_uart_rx_buffer));
    }

    /* copy the data */
    read_len = (len < uart2_Buffer.g_uart_point) ? len : uart2_Buffer.g_uart_point;

    memcpy(buf, uart2_Buffer.g_uart_rx_buffer, read_len);

    if( (buf[0] == '\r') || (buf[0] == '\n'))
    {
    	buf[0] = 0x20;
    }

    /* clear the buffer */
    uart2_Buffer.g_uart_point = 0;

    return read_len;
}

/**
 * @brief  uart flush data
 * @return status code
 *         - 0 success
 * @note   none
 */
uint16_t uart2_flush(void)
{
    /* clear the buffer */
	uart2_Buffer.g_uart_point = 0;

    return 0;
}

/**
 * @brief     uart print format data
 * @param[in] fmt format data
 * @return    length of the sent data
 * @note      none
 */
uint16_t uart2_print(const char *const fmt, ...)
{
    char str[256];
    uint16_t len;
    va_list args;

    /* print to the buffer */
    memset((char *)str, 0, sizeof(char) * 256);
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);

    /* send the data */
    len = strlen((char *)str);

    if(str[len-1] == '\n')
    {
    	str[len-1] = '\r';
    	str[len] = '\n';
    	str[len+1] = '\0';
    	len++;
    }

    if (uart2_write((uint8_t *)str, len) != 0)
    {
        return 0;
    }
    else
    {
        return len;
    }
}

/**
 * @brief  uart get the handle
 * @return pointer to a uart handle
 * @note   none
 */
UART_HandleTypeDef* uart2_get_handle(void)
{
    return &g_uart2_handle;
}


/**
 * @brief uart set tx done
 * @note  none
 */


void HAL_UART_TxCpltCallback(UART_HandleTypeDef* uart_handle)
{
	if(uart_handle == &huart2)
	{
		uart2_set_tx_done();
	}

}
void uart2_set_tx_done(void)
{
	uart2_Buffer.g_uart_tx_done = 1;
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef* uart_handle)
{
	if(uart_handle == &huart2)
	{
		uart2_irq_handler();
	}
	else
	{
		while(1)
		{
		}
	}

}


/**
 * @brief uart irq handler
 * @note  none
 */
void uart2_irq_handler(void)
{
    /* save one byte */
	uart2_Buffer.g_uart_rx_buffer[uart2_Buffer.g_uart_point] = uart2_Buffer.g_uart_buffer;
	uart2_Buffer.g_uart_point++;
    if (uart2_Buffer.g_uart_point > (UART_MAX_LEN - 1))
    {
    	uart2_Buffer.g_uart_point = 0;
    }

    /*
     * 버퍼의 비어있는 칸에, 방금 uart 레지스터에서 수신받은 값을 저장
     * 다음 빈칸을 가리키도록 point 증가
     *
     * 만약 버퍼 오버플로가 일어난다면, 처음부터 새로운 값을 읽도록 정의
     */

    /* receive one byte */
    (void)HAL_UART_Receive_IT(&g_uart2_handle, (uint8_t *)&uart2_Buffer.g_uart_buffer, 1);

    /*
     * 다음 1바이트 입력이 준비되면 인터럽트로 알려주도록 인터럽트를 켜둠
     *
     * 그리고 1byte의 데이터는 g_uart2_buffer 에 받아둔다.
     */

}

/* USER CODE END 1 */

