################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/components/libc/compilers/minilibc/ctype.c \
../rt-thread/components/libc/compilers/minilibc/math.c \
../rt-thread/components/libc/compilers/minilibc/qsort.c \
../rt-thread/components/libc/compilers/minilibc/rand.c \
../rt-thread/components/libc/compilers/minilibc/stdlib.c \
../rt-thread/components/libc/compilers/minilibc/string.c \
../rt-thread/components/libc/compilers/minilibc/time.c 

OBJS += \
./rt-thread/components/libc/compilers/minilibc/ctype.o \
./rt-thread/components/libc/compilers/minilibc/math.o \
./rt-thread/components/libc/compilers/minilibc/qsort.o \
./rt-thread/components/libc/compilers/minilibc/rand.o \
./rt-thread/components/libc/compilers/minilibc/stdlib.o \
./rt-thread/components/libc/compilers/minilibc/string.o \
./rt-thread/components/libc/compilers/minilibc/time.o 

C_DEPS += \
./rt-thread/components/libc/compilers/minilibc/ctype.d \
./rt-thread/components/libc/compilers/minilibc/math.d \
./rt-thread/components/libc/compilers/minilibc/qsort.d \
./rt-thread/components/libc/compilers/minilibc/rand.d \
./rt-thread/components/libc/compilers/minilibc/stdlib.d \
./rt-thread/components/libc/compilers/minilibc/string.d \
./rt-thread/components/libc/compilers/minilibc/time.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/components/libc/compilers/minilibc/%.o: ../rt-thread/components/libc/compilers/minilibc/%.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\RT-ThreadStudio\workspace\ARM_sensor\drivers" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\drivers\include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\drivers\include\config" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\libraries" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\libraries\CMSIS\Include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\RT-ThreadStudio\workspace\ARM_sensor" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\packages\freemodbus-latest\modbus\ascii" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\packages\freemodbus-latest\modbus\include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\packages\freemodbus-latest\modbus\rtu" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\packages\freemodbus-latest\port" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\components\drivers\include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\components\finsh" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\components\libc\compilers\minilibc" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\libcpu\arm\common" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\libcpu\arm\cortex-m3" -include"D:\RT-ThreadStudio\workspace\ARM_sensor\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

