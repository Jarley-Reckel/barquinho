################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/HMC5883L_dev.c \
../Core/Src/I2Cdev.c \
../Core/Src/actuators.c \
../Core/Src/boat_system.c \
../Core/Src/boat_tasks.c \
../Core/Src/delay.c \
../Core/Src/location_service.c \
../Core/Src/main.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/tools_and_types.c 

OBJS += \
./Core/Src/HMC5883L_dev.o \
./Core/Src/I2Cdev.o \
./Core/Src/actuators.o \
./Core/Src/boat_system.o \
./Core/Src/boat_tasks.o \
./Core/Src/delay.o \
./Core/Src/location_service.o \
./Core/Src/main.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/tools_and_types.o 

C_DEPS += \
./Core/Src/HMC5883L_dev.d \
./Core/Src/I2Cdev.d \
./Core/Src/actuators.d \
./Core/Src/boat_system.d \
./Core/Src/boat_tasks.d \
./Core/Src/delay.d \
./Core/Src/location_service.d \
./Core/Src/main.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/tools_and_types.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/HMC5883L_dev.cyclo ./Core/Src/HMC5883L_dev.d ./Core/Src/HMC5883L_dev.o ./Core/Src/HMC5883L_dev.su ./Core/Src/I2Cdev.cyclo ./Core/Src/I2Cdev.d ./Core/Src/I2Cdev.o ./Core/Src/I2Cdev.su ./Core/Src/actuators.cyclo ./Core/Src/actuators.d ./Core/Src/actuators.o ./Core/Src/actuators.su ./Core/Src/boat_system.cyclo ./Core/Src/boat_system.d ./Core/Src/boat_system.o ./Core/Src/boat_system.su ./Core/Src/boat_tasks.cyclo ./Core/Src/boat_tasks.d ./Core/Src/boat_tasks.o ./Core/Src/boat_tasks.su ./Core/Src/delay.cyclo ./Core/Src/delay.d ./Core/Src/delay.o ./Core/Src/delay.su ./Core/Src/location_service.cyclo ./Core/Src/location_service.d ./Core/Src/location_service.o ./Core/Src/location_service.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/tools_and_types.cyclo ./Core/Src/tools_and_types.d ./Core/Src/tools_and_types.o ./Core/Src/tools_and_types.su

.PHONY: clean-Core-2f-Src

