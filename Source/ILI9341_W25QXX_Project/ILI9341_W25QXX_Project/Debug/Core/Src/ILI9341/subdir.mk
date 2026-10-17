################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ILI9341/font.c \
../Core/Src/ILI9341/ili9341.c 

OBJS += \
./Core/Src/ILI9341/font.o \
./Core/Src/ILI9341/ili9341.o 

C_DEPS += \
./Core/Src/ILI9341/font.d \
./Core/Src/ILI9341/ili9341.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/ILI9341/%.o Core/Src/ILI9341/%.su Core/Src/ILI9341/%.cyclo: ../Core/Src/ILI9341/%.c Core/Src/ILI9341/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-ILI9341

clean-Core-2f-Src-2f-ILI9341:
	-$(RM) ./Core/Src/ILI9341/font.cyclo ./Core/Src/ILI9341/font.d ./Core/Src/ILI9341/font.o ./Core/Src/ILI9341/font.su ./Core/Src/ILI9341/ili9341.cyclo ./Core/Src/ILI9341/ili9341.d ./Core/Src/ILI9341/ili9341.o ./Core/Src/ILI9341/ili9341.su

.PHONY: clean-Core-2f-Src-2f-ILI9341

