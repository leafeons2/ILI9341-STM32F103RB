/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "w25qxx_with_ILI9341.h"

#include "shell.h"
#include "delay.h"

#include "stdio.h"
#include "stdlib.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

#define DEBUG_ME 1

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t g_buf[256];                        /**< uart buffer */
volatile uint16_t g_len;                   /**< uart buffer length */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */


  ILI9341_Init();
  SPI1_DeInit();

  /*
   * SPI 1을 볼 때, Init, 이용, Deinit 구조를 만족한다.
   * 앞으로 이 구조를 계속 유지해보고자 한다.
   */


  uint32_t  time_start =0;
  uint32_t  time_accumulate =0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//----------------------------------------------------------PERFORMANCE TEST
		static uint16_t x = 0;
		static uint16_t y = 0;

		uint8_t res;

		uint8_t some_GPIO_value =  GPIO_PIN_SET;
		/*
		 * SET 일시 사진 업로드 명령을 받을 수 있는 상태로 들어감
		 * RESET 일시 이미 되어있는 업로드를 기반으로 구동 상태로 들어감
		 */

		uart2_print("w25qxx: welcome to libdriver w25qxx.\n");


		if(some_GPIO_value ==  GPIO_PIN_RESET)
		{
			delay_init();
			shell_init();

			shell_register("w25qxx", w25qxx);


			/*
			char* test_string_1 = "w25qxx -e write --addr=0x100000 --data=0xAB";
			char* test_string_2 = "w25qxx -e read --addr=0x100000";
			char* test_string_3 = "w25qxx -e write --addr=0x100000 --data=0xCD";

			strncpy((char *)g_buf, test_string_1, strlen(test_string_1)+1);
			// strlen(g_buf) 가 제대로 동작하기 위해서는, 문자열 끝에 \0 문자가 들어와 있어야 한다.
			res = shell_parse((char *)g_buf, strlen((char *)g_buf));

			strncpy((char *)g_buf, test_string_2, strlen(test_string_1)+1);
			res = shell_parse((char *)g_buf, strlen((char *)g_buf));

			strncpy((char *)g_buf, test_string_3, strlen(test_string_1)+1);
			res = shell_parse((char *)g_buf, strlen((char *)g_buf));

			strncpy((char *)g_buf, test_string_2, strlen(test_string_1)+1);
			res = shell_parse((char *)g_buf, strlen((char *)g_buf));
			*/

			while (1)
			{
				/* read uart */
				g_len = uart_read(g_buf, 256, &g_uart2_handle);
				if (g_len != 0)
				{
					/* run shell */
					uart2_print("%s\n", g_buf);
					res = shell_parse((char *)g_buf, g_len);
					memset(g_buf,0, sizeof(g_buf));

					if (res == 0)
					{
						/* run success */
					}
					else if (res == 1)
					{
						uart2_print("w25qxx: run failed.\n");
					}
					else if (res == 2)
					{
						uart2_print("w25qxx: unknown command.\n");

					}
					else if (res == 3)
					{
						uart2_print("w25qxx: length is too long.\n");
					}
					else if (res == 4)
					{
						uart2_print("w25qxx: pretreat failed.\n");
					}
					else if (res == 5)
					{
						uart2_print("w25qxx: param is invalid.\n");
					}
					else
					{
						uart2_print("w25qxx: unknown status code.\n");
					}
					uart2_flush();
				}
				delay_ms(100);
			}
		}
		else
		{
#ifndef DEBUG_ME
			ILI9341_Fill_Screen(RED);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
			ILI9341_Draw_Text("FPS TEST, 40 loop 2 screens", 10, 10, BLACK, 1, WHITE);
			HAL_Delay(2000);
			ILI9341_Fill_Screen(WHITE);

			/*
			 * uint32_t Timer_Counter = 0;
			 */

			for (uint32_t j = 0; j < 2; j++)
			{
				time_start = HAL_GetTick();
				/*
				 * HAL_TIM_Base_Start(&htim1);
				 */

				for (uint16_t i = 0; i < 10; i++)
				{
					ILI9341_Fill_Screen(WHITE);
					ILI9341_Fill_Screen(BLACK);
				}

				//20.000 per second!

				time_accumulate += HAL_GetTick() - time_start;

				/*
				 HAL_TIM_Base_Stop(&htim1);
				 Timer_Counter += __HAL_TIM_GET_COUNTER(&htim1);
				 __HAL_TIM_SET_COUNTER(&htim1, 0);
				 */

			}

			char counter_buff[30];
			ILI9341_Fill_Screen(WHITE);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
			sprintf(counter_buff, "Elapsed Time(ms) : %d", time_accumulate);
			ILI9341_Draw_Text(counter_buff, 10, 10, BLACK, 1, WHITE);

			double seconds_passed = time_accumulate / 1000;
			sprintf(counter_buff, "Time: %.3f Sec", seconds_passed);
			ILI9341_Draw_Text(counter_buff, 10, 30, BLACK, 2, WHITE);

			double timer_float = 40 / ( time_accumulate / 1000); //Frames per sec

			sprintf(counter_buff, "FPS:  %.2f", timer_float);
			ILI9341_Draw_Text(counter_buff, 10, 50, BLACK, 2, WHITE);

			double MB_PS = timer_float * 240 * 320 * 2 / 1000000;
			sprintf(counter_buff, "MB/S: %.2f", MB_PS);
			ILI9341_Draw_Text(counter_buff, 10, 70, BLACK, 2, WHITE);

			/*
			 *  (n 장 / 1초)   *  (240 * 320 pixel  / 1장)  *  (2byte /1 pixel)   *   (1MB / 1000000Byte)
			 *     = n MB / 초
			 */

			double SPI_utilized_percentage = (MB_PS / (6.25)) * 100;//50mbits / 8 bits
			sprintf(counter_buff, "SPI Utilized: %.2f", SPI_utilized_percentage);
			ILI9341_Draw_Text(counter_buff, 10, 90, BLACK, 2, WHITE);

			/*
			 *  SPI 의 최대 통신 가능 MB/s 는 6.25 MB/s 이다.  이것과 얼마나 가까운지를 비율로 표현
			 */

			HAL_Delay(10000);
			/*
			 Timer_Counter /= 2;


			 char counter_buff[30];
			 ILI9341_Fill_Screen(WHITE);
			 ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
			 sprintf(counter_buff, "Timer counter value: %d", Timer_Counter * 2);
			 ILI9341_Draw_Text(counter_buff, 10, 10, BLACK, 1, WHITE);

			 double seconds_passed = 2 * ((float) Timer_Counter / 20000);
			 sprintf(counter_buff, "Time: %.3f Sec", seconds_passed);
			 ILI9341_Draw_Text(counter_buff, 10, 30, BLACK, 2, WHITE);

			 double timer_float = 20 / (((float) Timer_Counter) / 20000);//Frames per sec

			 sprintf(counter_buff, "FPS:  %.2f", timer_float);
			 ILI9341_Draw_Text(counter_buff, 10, 50, BLACK, 2, WHITE);
			 double MB_PS = timer_float * 240 * 320 * 2 / 1000000;
			 sprintf(counter_buff, "MB/S: %.2f", MB_PS);
			 ILI9341_Draw_Text(counter_buff, 10, 70, BLACK, 2, WHITE);
			 double SPI_utilized_percentage = (MB_PS / (6.25)) * 100;//50mbits / 8 bits
			 sprintf(counter_buff, "SPI Utilized: %.2f", SPI_utilized_percentage);
			 ILI9341_Draw_Text(counter_buff, 10, 90, BLACK, 2, WHITE);
			 HAL_Delay(10000);

			 */

			char Temp_Buffer_text[40];

			//----------------------------------------------------------COUNTING MULTIPLE SEGMENTS
			ILI9341_Fill_Screen(WHITE);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
			ILI9341_Draw_Text("Counting multiple segments at once", 10, 10, BLACK, 1, WHITE);
			HAL_Delay(2000);
			ILI9341_Fill_Screen(WHITE);

			for (uint16_t i = 0; i <= 10; i++)
			{
				sprintf(Temp_Buffer_text, "Counting: %d", i);
				ILI9341_Draw_Text(Temp_Buffer_text, 10, 10, BLACK, 2, WHITE);
				ILI9341_Draw_Text(Temp_Buffer_text, 10, 30, BLUE, 2, WHITE);
				ILI9341_Draw_Text(Temp_Buffer_text, 10, 50, RED, 2, WHITE);
				ILI9341_Draw_Text(Temp_Buffer_text, 10, 70, GREEN, 2, WHITE);
				ILI9341_Draw_Text(Temp_Buffer_text, 10, 90, BLACK, 2, WHITE);
				ILI9341_Draw_Text(Temp_Buffer_text, 10, 110, BLUE, 2, WHITE);
				ILI9341_Draw_Text(Temp_Buffer_text, 10, 130, RED, 2, WHITE);
				ILI9341_Draw_Text(Temp_Buffer_text, 10, 150, GREEN, 2, WHITE);
				ILI9341_Draw_Text(Temp_Buffer_text, 10, 170, WHITE, 2, BLACK);
				ILI9341_Draw_Text(Temp_Buffer_text, 10, 190, BLUE, 2, BLACK);
				ILI9341_Draw_Text(Temp_Buffer_text, 10, 210, RED, 2, BLACK);
			}

			HAL_Delay(1000);

			//----------------------------------------------------------COUNTING SINGLE SEGMENT
			ILI9341_Fill_Screen(WHITE);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
			ILI9341_Draw_Text("Counting single segment", 10, 10, BLACK, 1, WHITE);
			HAL_Delay(2000);
			ILI9341_Fill_Screen(WHITE);

			for (uint16_t i = 0; i <= 100; i++)
			{
				sprintf(Temp_Buffer_text, "Counting: %d", i);
				ILI9341_Draw_Text(Temp_Buffer_text, 10, 10, BLACK, 3, WHITE);
			}

			HAL_Delay(1000);

			//----------------------------------------------------------ALIGNMENT TEST
			ILI9341_Fill_Screen(WHITE);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
			ILI9341_Draw_Text("Rectangle alignment check", 10, 10, BLACK, 1, WHITE);
			HAL_Delay(2000);
			ILI9341_Fill_Screen(WHITE);

			ILI9341_Draw_Hollow_Rectangle_Coord(50, 50, 100, 100, BLACK);
			ILI9341_Draw_Filled_Rectangle_Coord(20, 20, 50, 50, BLACK);
			ILI9341_Draw_Hollow_Rectangle_Coord(10, 10, 19, 19, BLACK);
			HAL_Delay(1000);

			//----------------------------------------------------------LINES EXAMPLE
			ILI9341_Fill_Screen(WHITE);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
			ILI9341_Draw_Text("Randomly placed and sized", 10, 10, BLACK, 1, WHITE);
			ILI9341_Draw_Text("Horizontal and Vertical lines", 10, 20, BLACK, 1, WHITE);
			HAL_Delay(2000);
			ILI9341_Fill_Screen(WHITE);

			srand(HAL_GetTick());

			for (uint32_t i = 0; i < 10000; i++)
			{
				/*
				 * uint32_t random_num = 0;
				 */
				uint16_t xr = 0;
				uint16_t yr = 0;
				uint16_t radiusr = 0;
				uint16_t colourr = 0;

				xr = rand();
				yr = rand();
				radiusr = rand();
				colourr = rand();

				xr %= 320;
				yr %= 240;
				radiusr &= 0x001F;
				ILI9341_Draw_Horizontal_Line(xr, yr, radiusr, colourr);
				ILI9341_Draw_Vertical_Line(xr, yr, radiusr, colourr);

				/*
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 xr = random_num;
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 yr = random_num;
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 radiusr = random_num;
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 colourr = random_num;

				 xr &= 0x01FF;
				 yr &= 0x01FF;
				 radiusr &= 0x001F;
				 //ili9341_drawpixel(xr, yr, WHITE);
				 ILI9341_Draw_Horizontal_Line(xr, yr, radiusr, colourr);
				 ILI9341_Draw_Vertical_Line(xr, yr, radiusr, colourr);

				 */

			}

			HAL_Delay(1000);

			//----------------------------------------------------------HOLLOW CIRCLES EXAMPLE
			ILI9341_Fill_Screen(WHITE);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
			ILI9341_Draw_Text("Randomly placed and sized", 10, 10, BLACK, 1, WHITE);
			ILI9341_Draw_Text("Circles", 10, 20, BLACK, 1, WHITE);
			HAL_Delay(2000);
			ILI9341_Fill_Screen(WHITE);

			for (uint32_t i = 0; i < 3000; i++)
			{
				/*
				 uint32_t random_num = 0;
				 */

				uint16_t xr = 0;
				uint16_t yr = 0;
				uint16_t radiusr = 0;
				uint16_t colourr = 0;

				xr = rand();
				yr = rand();
				radiusr = rand();
				colourr = rand();

				/*
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 xr = random_num;
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 yr = random_num;
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 radiusr = random_num;
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 colourr = random_num;
				 */

				xr &= 0x01FF;
				yr &= 0x01FF;
				radiusr &= 0x001F;
				//ili9341_drawpixel(xr, yr, WHITE);
				ILI9341_Draw_Hollow_Circle(xr, yr, radiusr * 2, colourr);

			}
			HAL_Delay(1000);

			//----------------------------------------------------------FILLED CIRCLES EXAMPLE
			ILI9341_Fill_Screen(WHITE);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
			ILI9341_Draw_Text("Randomly placed and sized", 10, 10, BLACK, 1, WHITE);
			ILI9341_Draw_Text("Filled Circles", 10, 20, BLACK, 1, WHITE);
			HAL_Delay(2000);
			ILI9341_Fill_Screen(WHITE);

			for (uint32_t i = 0; i < 1000; i++)
			{
				/*
				 uint32_t random_num = 0;
				 */

				uint16_t xr = 0;
				uint16_t yr = 0;
				uint16_t radiusr = 0;
				uint16_t colourr = 0;

				xr = rand();
				yr = rand();
				radiusr = rand();
				colourr = rand();

				/*
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 xr = random_num;
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 yr = random_num;
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 radiusr = random_num;
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 colourr = random_num;
				 */

				xr &= 0x01FF;
				yr &= 0x01FF;
				radiusr &= 0x001F;
				//ili9341_drawpixel(xr, yr, WHITE);
				ILI9341_Draw_Filled_Circle(xr, yr, radiusr / 2, colourr);
			}

			HAL_Delay(1000);

			//----------------------------------------------------------HOLLOW RECTANGLES EXAMPLE
			ILI9341_Fill_Screen(WHITE);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
			ILI9341_Draw_Text("Randomly placed and sized", 10, 10, BLACK, 1, WHITE);
			ILI9341_Draw_Text("Rectangles", 10, 20, BLACK, 1, WHITE);
			HAL_Delay(2000);
			ILI9341_Fill_Screen(WHITE);

			for (uint32_t i = 0; i < 20000; i++)
			{
				/*
				 uint32_t random_num = 0;
				 */

				uint16_t xr = 0;
				uint16_t yr = 0;
				uint16_t radiusr = 0;
				uint16_t colourr = 0;

				xr = rand();
				yr = rand();
				radiusr = rand();
				colourr = rand();

				/*
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 xr = random_num;
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 yr = random_num;
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 radiusr = random_num;
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 colourr = random_num;
				 */

				xr &= 0x01FF;
				yr &= 0x01FF;
				radiusr &= 0x001F;
				//ili9341_drawpixel(xr, yr, WHITE);
				ILI9341_Draw_Hollow_Rectangle_Coord(xr, yr, xr + radiusr,
						yr + radiusr, colourr);
			}
			HAL_Delay(1000);

			//----------------------------------------------------------FILLED RECTANGLES EXAMPLE
			ILI9341_Fill_Screen(WHITE);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
			ILI9341_Draw_Text("Randomly placed and sized", 10, 10, BLACK, 1, WHITE);
			ILI9341_Draw_Text("Filled Rectangles", 10, 20, BLACK, 1, WHITE);
			HAL_Delay(2000);
			ILI9341_Fill_Screen(WHITE);

			for (uint32_t i = 0; i < 20000; i++)
			{
				/*
				 uint32_t random_num = 0;
				 */

				uint16_t xr = 0;
				uint16_t yr = 0;
				uint16_t radiusr = 0;
				uint16_t colourr = 0;

				xr = rand();
				yr = rand();
				radiusr = rand();
				colourr = rand();

				/*
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 xr = random_num;
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 yr = random_num;
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 radiusr = random_num;
				 random_num = HAL_RNG_GetRandomNumber(&hrng);
				 colourr = random_num;
				 */

				xr &= 0x01FF;
				yr &= 0x01FF;
				radiusr &= 0x001F;
				//ili9341_drawpixel(xr, yr, WHITE);
				ILI9341_Draw_Rectangle(xr, yr, radiusr, radiusr, colourr);
			}
			HAL_Delay(1000);

			//----------------------------------------------------------INDIVIDUAL PIXEL EXAMPLE

			ILI9341_Fill_Screen(WHITE);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
			ILI9341_Draw_Text("Slow draw by selecting", 10, 10, BLACK, 1, WHITE);
			ILI9341_Draw_Text("and adressing pixels", 10, 20, BLACK, 1, WHITE);
			HAL_Delay(2000);
			ILI9341_Fill_Screen(WHITE);

			x = 0;
			y = 0;
			while (y < 240)
			{
				while ((x < 320) && (y < 240))
				{

					if (x % 2)
					{
						ILI9341_Draw_Pixel(x, y, BLACK);
					}

					x++;
				}

				y++;
				x = 0;
			}

			x = 0;
			y = 0;

			while (y < 240)
			{
				while ((x < 320) && (y < 240))
				{

					if (y % 2)
					{
						ILI9341_Draw_Pixel(x, y, BLACK);
					}

					x++;
				}

				y++;
				x = 0;
			}
			HAL_Delay(2000);

			//----------------------------------------------------------INDIVIDUAL PIXEL EXAMPLE
			/*
			 ILI9341_Fill_Screen(WHITE);
			 ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
			 ILI9341_Draw_Text("Random position and colour", 10, 10, BLACK, 1,
			 WHITE);
			 ILI9341_Draw_Text("500000 pixels", 10, 20, BLACK, 1, WHITE);
			 HAL_Delay(2000);
			 ILI9341_Fill_Screen(WHITE);

			 for (uint32_t i = 0; i < 500000; i++)
			 {
			 uint32_t random_num = 0;
			 uint16_t xr = 0;
			 uint16_t yr = 0;
			 random_num = HAL_RNG_GetRandomNumber(&hrng);
			 xr = random_num;
			 random_num = HAL_RNG_GetRandomNumber(&hrng);
			 yr = random_num;
			 uint16_t color = HAL_RNG_GetRandomNumber(&hrng);

			 xr &= 0x01FF;
			 yr &= 0x01FF;
			 ILI9341_Draw_Pixel(xr, yr, color);
			 }
			 HAL_Delay(2000);
			 */

			//----------------------------------------------------------565 COLOUR EXAMPLE, Grayscale
			ILI9341_Fill_Screen(WHITE);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
			ILI9341_Draw_Text("Colour gradient", 10, 10, BLACK, 1, WHITE);
			ILI9341_Draw_Text("Grayscale", 10, 20, BLACK, 1, WHITE);
			HAL_Delay(2000);

			for (uint16_t i = 0; i <= (320); i++)
			{
				uint16_t Red = 0;
				uint16_t Green = 0;
				uint16_t Blue = 0;

				Red = i / (10);
				Red <<= 11;
				Green = i / (5);
				Green <<= 5;
				Blue = i / (10);

				uint16_t RGB_color = Red + Green + Blue;
				ILI9341_Draw_Rectangle(i, x, 1, 240, RGB_color);

			}
			HAL_Delay(2000);
#endif

			//----------------------------------------------------------IMAGE EXAMPLE, Snow Tiger

			MX_SPI1_Init();
			ILI9341_Fill_Screen(WHITE);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
			ILI9341_Draw_Text("Loading Up snow_Tiger and others", 10, 10, BLACK, 1, WHITE);
			SPI1_DeInit();


			/*
			 *  여기서 또한 SPI1을 초기화 한후, 이용하고, deinit 까지 거친다.
			 *  deinit 까지 안한 상태로 냅둬도 되지만, 아래 함수인 W25QXX_WITH_ILI9341_Draw_Image에서 자체적으로
			 *  init 과정이 존재한다.   따라서 deinit 까지 해서, init에 문제없도록 말끔히 정리
			 */


			HAL_Delay(2000);

			res = W25QXX_WITH_ILI9341_Draw_Image(0x100000, SCREEN_VERTICAL_2);
			if(res == 1)
			{
				uart2_print("there was an error while initing w25qxx - snow tiger\n");
			}
			else if(res == 2)
			{
				uart2_print("there was an error while receiving info from w25qxx -snow tiger\n");
			}
			else  // return 0;
			{
				uart2_print("finished uploading snow tiger\n");
			}


			while(1)
			{
				res = W25QXX_WITH_ILI9341_Draw_Image(0x100000, SCREEN_VERTICAL_2);
				res = W25QXX_WITH_ILI9341_Draw_Image(0x130000, SCREEN_VERTICAL_2);
				res = W25QXX_WITH_ILI9341_Draw_Image(0x160000, SCREEN_VERTICAL_2);
				res = W25QXX_WITH_ILI9341_Draw_Image(0x190000, SCREEN_VERTICAL_2);
				res = W25QXX_WITH_ILI9341_Draw_Image(0x1C0000, SCREEN_VERTICAL_2);
				res = W25QXX_WITH_ILI9341_Draw_Image(0x1F0000, SCREEN_VERTICAL_2);
				res = W25QXX_WITH_ILI9341_Draw_Image(0x220000, SCREEN_VERTICAL_2);
				HAL_Delay(500);
			}

		}


		HAL_Delay(30000);



		/*
		 ILI9341_Draw_Image((const char*) snow_tiger, SCREEN_VERTICAL_2);
		 ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
		 HAL_Delay(10000);

		 *  용량 문제로 snow Tiger를 담을 수 없음.
		 *
		 */


		//----------------------------------------------------------TOUCHSCREEN EXAMPLE
		MX_SPI1_Init();

		ILI9341_Fill_Screen(WHITE);
		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
		ILI9341_Draw_Text("Touchscreen", 10, 10, BLACK, 2, WHITE);
		ILI9341_Draw_Text("Touch to draw", 10, 30, BLACK, 2, WHITE);
		ILI9341_Set_Rotation(SCREEN_VERTICAL_1);

		HAL_GPIO_WritePin(DIS_CS_GPIO_Port, DIS_CS_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(T_CS_GPIO_Port, T_CS_Pin, GPIO_PIN_SET);

		/*
		 *  Polling 방식으로 터치 검사
		 */


		while (1)
		{

			for (int i=0; i<5; i++);

			if (TP_Touchpad_Pressed())   //  T_IRQ 라 이름붙은 포트, 핀을 상시 검사
			{
				uint16_t x_pos = 0;
				uint16_t y_pos = 0;

				/*
				HAL_GPIO_WritePin(GPIOB, LD3_Pin | LD2_Pin, GPIO_PIN_SET);  //  보드 위 LED, 디스플레의 LED를 건드는 것 같음
				 */

				uint16_t position_array[2];

				if (TP_Read_Coordinates(position_array) == TOUCHPAD_DATA_OK)
				{
					x_pos = position_array[0];
					y_pos = position_array[1];
					ILI9341_Draw_Filled_Circle(x_pos, y_pos, 10, BLACK);

					ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
					char counter_buff[30];
					sprintf(counter_buff, "POS X: %.3d", x_pos);
					ILI9341_Draw_Text(counter_buff, 10, 80, BLACK, 2, WHITE);
					sprintf(counter_buff, "POS Y: %.3d", y_pos);
					ILI9341_Draw_Text(counter_buff, 10, 120, BLACK, 2, WHITE);
					ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
				}

				//ILI9341_Draw_Pixel(x_pos, y_pos, BLACK);

			}
			else
			{
				 // HAL_GPIO_WritePin(GPIOB, LD3_Pin | LD2_Pin, GPIO_PIN_RESET);

			}

		}

	}


  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
		/* User can add his own implementation to report the HAL error return state */
		__disable_irq();
		while (1)
		{

		}
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
