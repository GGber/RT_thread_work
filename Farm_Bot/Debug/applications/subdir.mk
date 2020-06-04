################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../applications/AD7995.c \
../applications/Slave.c \
../applications/Zigbee.c \
../applications/main.c 

OBJS += \
./applications/AD7995.o \
./applications/Slave.o \
./applications/Zigbee.o \
./applications/main.o 

C_DEPS += \
./applications/AD7995.d \
./applications/Slave.d \
./applications/Zigbee.d \
./applications/main.d 


# Each subdirectory must supply rules for building sources it contributes
applications/%.o: ../applications/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\RT-ThreadStudio\workspace\Farm013\drivers" -I"D:\RT-ThreadStudio\workspace\Farm013\libraries" -I"D:\RT-ThreadStudio\workspace\Farm013\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\RT-ThreadStudio\workspace\Farm013\libraries\CMSIS\Include" -I"D:\RT-ThreadStudio\workspace\Farm013\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\RT-ThreadStudio\workspace\Farm013" -I"D:\RT-ThreadStudio\workspace\Farm013\packages\freemodbus-latest\modbus\ascii" -I"D:\RT-ThreadStudio\workspace\Farm013\packages\freemodbus-latest\modbus\include" -I"D:\RT-ThreadStudio\workspace\Farm013\packages\freemodbus-latest\modbus\rtu" -I"D:\RT-ThreadStudio\workspace\Farm013\packages\freemodbus-latest\port" -I"D:\RT-ThreadStudio\workspace\Farm013\rt-thread\components\drivers\include" -I"D:\RT-ThreadStudio\workspace\Farm013\rt-thread\components\finsh" -I"D:\RT-ThreadStudio\workspace\Farm013\rt-thread\components\libc\compilers\minilibc" -I"D:\RT-ThreadStudio\workspace\Farm013\rt-thread\include" -I"D:\RT-ThreadStudio\workspace\Farm013\rt-thread\libcpu\arm\common" -I"D:\RT-ThreadStudio\workspace\Farm013\rt-thread\libcpu\arm\cortex-m3" -include"D:\RT-ThreadStudio\workspace\Farm013\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


