################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../packages/freemodbus-latest/port/port.c \
../packages/freemodbus-latest/port/portevent.c \
../packages/freemodbus-latest/port/portserial.c \
../packages/freemodbus-latest/port/porttimer.c \
../packages/freemodbus-latest/port/user_mb_app.c 

OBJS += \
./packages/freemodbus-latest/port/port.o \
./packages/freemodbus-latest/port/portevent.o \
./packages/freemodbus-latest/port/portserial.o \
./packages/freemodbus-latest/port/porttimer.o \
./packages/freemodbus-latest/port/user_mb_app.o 

C_DEPS += \
./packages/freemodbus-latest/port/port.d \
./packages/freemodbus-latest/port/portevent.d \
./packages/freemodbus-latest/port/portserial.d \
./packages/freemodbus-latest/port/porttimer.d \
./packages/freemodbus-latest/port/user_mb_app.d 


# Each subdirectory must supply rules for building sources it contributes
packages/freemodbus-latest/port/%.o: ../packages/freemodbus-latest/port/%.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\RT-ThreadStudio\workspace\ARM_sensor\drivers" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\drivers\include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\drivers\include\config" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\libraries" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\libraries\CMSIS\Include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\RT-ThreadStudio\workspace\ARM_sensor" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\packages\freemodbus-latest\modbus\ascii" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\packages\freemodbus-latest\modbus\include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\packages\freemodbus-latest\modbus\rtu" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\packages\freemodbus-latest\port" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\components\drivers\include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\components\finsh" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\components\libc\compilers\minilibc" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\libcpu\arm\common" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\libcpu\arm\cortex-m3" -include"D:\RT-ThreadStudio\workspace\ARM_sensor\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

