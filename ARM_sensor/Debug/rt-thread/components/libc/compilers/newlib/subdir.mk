################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/components/libc/compilers/newlib/libc.c \
../rt-thread/components/libc/compilers/newlib/libc_syms.c \
../rt-thread/components/libc/compilers/newlib/stdio.c \
../rt-thread/components/libc/compilers/newlib/syscalls.c \
../rt-thread/components/libc/compilers/newlib/time.c 

OBJS += \
./rt-thread/components/libc/compilers/newlib/libc.o \
./rt-thread/components/libc/compilers/newlib/libc_syms.o \
./rt-thread/components/libc/compilers/newlib/stdio.o \
./rt-thread/components/libc/compilers/newlib/syscalls.o \
./rt-thread/components/libc/compilers/newlib/time.o 

C_DEPS += \
./rt-thread/components/libc/compilers/newlib/libc.d \
./rt-thread/components/libc/compilers/newlib/libc_syms.d \
./rt-thread/components/libc/compilers/newlib/stdio.d \
./rt-thread/components/libc/compilers/newlib/syscalls.d \
./rt-thread/components/libc/compilers/newlib/time.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/components/libc/compilers/newlib/%.o: ../rt-thread/components/libc/compilers/newlib/%.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\RT-ThreadStudio\workspace\ARM_sensor\drivers" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\drivers\include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\drivers\include\config" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\libraries" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\libraries\CMSIS\Include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\RT-ThreadStudio\workspace\ARM_sensor" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\packages\libmodbus-latest\examples" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\packages\libmodbus-latest\inc" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\components\dfs\filesystems\devfs" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\components\dfs\include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\components\drivers\include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\components\finsh" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\components\libc\compilers\newlib" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\components\libc\termios" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\include" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\libcpu\arm\common" -I"D:\RT-ThreadStudio\workspace\ARM_sensor\rt-thread\libcpu\arm\cortex-m3" -include"D:\RT-ThreadStudio\workspace\ARM_sensor\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

