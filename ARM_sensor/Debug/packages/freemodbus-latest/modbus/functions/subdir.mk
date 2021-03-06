################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../packages/freemodbus-latest/modbus/functions/mbfunccoils.c \
../packages/freemodbus-latest/modbus/functions/mbfuncdiag.c \
../packages/freemodbus-latest/modbus/functions/mbfuncdisc.c \
../packages/freemodbus-latest/modbus/functions/mbfuncholding.c \
../packages/freemodbus-latest/modbus/functions/mbfuncinput.c \
../packages/freemodbus-latest/modbus/functions/mbfuncother.c \
../packages/freemodbus-latest/modbus/functions/mbutils.c 

OBJS += \
./packages/freemodbus-latest/modbus/functions/mbfunccoils.o \
./packages/freemodbus-latest/modbus/functions/mbfuncdiag.o \
./packages/freemodbus-latest/modbus/functions/mbfuncdisc.o \
./packages/freemodbus-latest/modbus/functions/mbfuncholding.o \
./packages/freemodbus-latest/modbus/functions/mbfuncinput.o \
./packages/freemodbus-latest/modbus/functions/mbfuncother.o \
./packages/freemodbus-latest/modbus/functions/mbutils.o 

C_DEPS += \
./packages/freemodbus-latest/modbus/functions/mbfunccoils.d \
./packages/freemodbus-latest/modbus/functions/mbfuncdiag.d \
./packages/freemodbus-latest/modbus/functions/mbfuncdisc.d \
./packages/freemodbus-latest/modbus/functions/mbfuncholding.d \
./packages/freemodbus-latest/modbus/functions/mbfuncinput.d \
./packages/freemodbus-latest/modbus/functions/mbfuncother.d \
./packages/freemodbus-latest/modbus/functions/mbutils.d 


# Each subdirectory must supply rules for building sources it contributes
packages/freemodbus-latest/modbus/functions/%.o: ../packages/freemodbus-latest/modbus/functions/%.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\RT-ThreadStudio\workspace\ARM_sensor\drivers" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\drivers\include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\drivers\include\config" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\libraries" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\libraries\CMSIS\Include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\RT-ThreadStudio\workspace\ARM_sensor" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\packages\freemodbus-latest\modbus\ascii" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\packages\freemodbus-latest\modbus\include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\packages\freemodbus-latest\modbus\rtu" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\packages\freemodbus-latest\port" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\components\drivers\include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\components\finsh" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\components\libc\compilers\minilibc" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\libcpu\arm\common" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\libcpu\arm\cortex-m3" -include"D:\RT-ThreadStudio\workspace\ARM_sensor\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

