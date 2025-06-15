################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../UUT/Src/adc_test.c \
../UUT/Src/hw_verif.c \
../UUT/Src/hw_verif_crc.c \
../UUT/Src/hw_verif_udp.c \
../UUT/Src/i2c_test.c \
../UUT/Src/spi_test.c \
../UUT/Src/tests.c \
../UUT/Src/timer_test.c \
../UUT/Src/uart_test.c 

OBJS += \
./UUT/Src/adc_test.o \
./UUT/Src/hw_verif.o \
./UUT/Src/hw_verif_crc.o \
./UUT/Src/hw_verif_udp.o \
./UUT/Src/i2c_test.o \
./UUT/Src/spi_test.o \
./UUT/Src/tests.o \
./UUT/Src/timer_test.o \
./UUT/Src/uart_test.o 

C_DEPS += \
./UUT/Src/adc_test.d \
./UUT/Src/hw_verif.d \
./UUT/Src/hw_verif_crc.d \
./UUT/Src/hw_verif_udp.d \
./UUT/Src/i2c_test.d \
./UUT/Src/spi_test.d \
./UUT/Src/tests.d \
./UUT/Src/timer_test.d \
./UUT/Src/uart_test.d 


# Each subdirectory must supply rules for building sources it contributes
UUT/Src/%.o UUT/Src/%.su UUT/Src/%.cyclo: ../UUT/Src/%.c UUT/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F756xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../UUT/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-UUT-2f-Src

clean-UUT-2f-Src:
	-$(RM) ./UUT/Src/adc_test.cyclo ./UUT/Src/adc_test.d ./UUT/Src/adc_test.o ./UUT/Src/adc_test.su ./UUT/Src/hw_verif.cyclo ./UUT/Src/hw_verif.d ./UUT/Src/hw_verif.o ./UUT/Src/hw_verif.su ./UUT/Src/hw_verif_crc.cyclo ./UUT/Src/hw_verif_crc.d ./UUT/Src/hw_verif_crc.o ./UUT/Src/hw_verif_crc.su ./UUT/Src/hw_verif_udp.cyclo ./UUT/Src/hw_verif_udp.d ./UUT/Src/hw_verif_udp.o ./UUT/Src/hw_verif_udp.su ./UUT/Src/i2c_test.cyclo ./UUT/Src/i2c_test.d ./UUT/Src/i2c_test.o ./UUT/Src/i2c_test.su ./UUT/Src/spi_test.cyclo ./UUT/Src/spi_test.d ./UUT/Src/spi_test.o ./UUT/Src/spi_test.su ./UUT/Src/tests.cyclo ./UUT/Src/tests.d ./UUT/Src/tests.o ./UUT/Src/tests.su ./UUT/Src/timer_test.cyclo ./UUT/Src/timer_test.d ./UUT/Src/timer_test.o ./UUT/Src/timer_test.su ./UUT/Src/uart_test.cyclo ./UUT/Src/uart_test.d ./UUT/Src/uart_test.o ./UUT/Src/uart_test.su

.PHONY: clean-UUT-2f-Src

