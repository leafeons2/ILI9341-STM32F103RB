################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_basic.c \
../Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_read_test.c \
../Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_register_test.c 

OBJS += \
./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_basic.o \
./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_read_test.o \
./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_register_test.o 

C_DEPS += \
./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_basic.d \
./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_read_test.d \
./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_register_test.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/W25Q64_UserCodfe_Src/take_this/%.o Core/Src/W25Q64_UserCodfe_Src/take_this/%.su Core/Src/W25Q64_UserCodfe_Src/take_this/%.cyclo: ../Core/Src/W25Q64_UserCodfe_Src/take_this/%.c Core/Src/W25Q64_UserCodfe_Src/take_this/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Src/W25Q64_UserCodfe_Src/take_this/w25qxx_interface -I../Core/Src/W25Q64_UserCodfe_Src/take_this -I../Core/Src/W25Q64_UserCodfe_Src/take_this/SPI_interface -I"../Core/Src/ILI9341_UserCode_Src/ILI9341" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-W25Q64_UserCodfe_Src-2f-take_this

clean-Core-2f-Src-2f-W25Q64_UserCodfe_Src-2f-take_this:
	-$(RM) ./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_basic.cyclo ./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_basic.d ./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_basic.o ./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_basic.su ./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_read_test.cyclo ./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_read_test.d ./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_read_test.o ./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_read_test.su ./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_register_test.cyclo ./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_register_test.d ./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_register_test.o ./Core/Src/W25Q64_UserCodfe_Src/take_this/driver_w25qxx_register_test.su

.PHONY: clean-Core-2f-Src-2f-W25Q64_UserCodfe_Src-2f-take_this

