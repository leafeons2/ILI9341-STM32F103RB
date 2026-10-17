################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/driver_w25qxx.c \
../Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/driver_w25qxx_interface.c 

OBJS += \
./Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/driver_w25qxx.o \
./Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/driver_w25qxx_interface.o 

C_DEPS += \
./Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/driver_w25qxx.d \
./Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/driver_w25qxx_interface.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/%.o Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/%.su Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/%.cyclo: ../Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/%.c Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface -I../Core/Src/W25Q64_UserCodfe_Src/take_this -I../Core/Src/W25Q64_UserCodfe_Src/take_this/SPI_interface -I"../Core/Src/ILI9341_UserCode_Src/ILI9341" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-W25Q64_UserCodfe_Src-2f-take_this-2f-w25qxx_interface

clean-Core-2f-Src-2f-W25Q64_UserCodfe_Src-2f-take_this-2f-w25qxx_interface:
	-$(RM) ./Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/driver_w25qxx.cyclo ./Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/driver_w25qxx.d ./Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/driver_w25qxx.o ./Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/driver_w25qxx.su ./Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/driver_w25qxx_interface.cyclo ./Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/driver_w25qxx_interface.d ./Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/driver_w25qxx_interface.o ./Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface/driver_w25qxx_interface.su

.PHONY: clean-Core-2f-Src-2f-W25Q64_UserCodfe_Src-2f-take_this-2f-w25qxx_interface

