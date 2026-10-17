

#include "driver_w25qxx_basic.h"
#include "driver_w25qxx_read_test.h"
#include "driver_w25qxx_register_test.h"

#include "ILI9341_Touchscreen.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"

#include "getopt.h"
#include "math.h"

#include <stdlib.h>
#include <errno.h>

#include "usart.h"
#include "delay.h"

typedef enum
{
	STREAM_IDLE = 0x00u,
	STREAM_INITED = 0x01u,
	STREAM_INUSE_NOERROR = 0x02u,
	STREAM_INUSE_ERROR_MERGEATTEMPT = 0x03u,
	STREAM_INUSE_ERROR_MERGEFAIL_FILLINGZERO = 0x04u,
	STREAM_INUSE_NOERROR_NOINPUT_FORLONG = 0x05u
} STREAM_StatusTypeDef;



uint8_t w25qxx(uint8_t argc, char **argv);

uint8_t W25QXX_WITH_ILI9341_Draw_Image(uint32_t addr, uint8_t orientation);

uint8_t hex_to_uint32(const char *str, uint32_t *result);

uint8_t  w25qxx_FastRead (uint32_t addr, uint8_t *receive_buffer, uint32_t receive_ammount);
