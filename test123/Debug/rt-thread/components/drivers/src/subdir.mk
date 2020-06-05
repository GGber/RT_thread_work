################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/components/drivers/src/completion.c \
../rt-thread/components/drivers/src/dataqueue.c \
../rt-thread/components/drivers/src/pipe.c \
../rt-thread/components/drivers/src/ringblk_buf.c \
../rt-thread/components/drivers/src/ringbuffer.c \
../rt-thread/components/drivers/src/waitqueue.c \
../rt-thread/components/drivers/src/workqueue.c 

OBJS += \
./rt-thread/components/drivers/src/completion.o \
./rt-thread/components/drivers/src/dataqueue.o \
./rt-thread/components/drivers/src/pipe.o \
./rt-thread/components/drivers/src/ringblk_buf.o \
./rt-thread/components/drivers/src/ringbuffer.o \
./rt-thread/components/drivers/src/waitqueue.o \
./rt-thread/components/drivers/src/workqueue.o 

C_DEPS += \
./rt-thread/components/drivers/src/completion.d \
./rt-thread/components/drivers/src/dataqueue.d \
./rt-thread/components/drivers/src/pipe.d \
./rt-thread/components/drivers/src/ringblk_buf.d \
./rt-thread/components/drivers/src/ringbuffer.d \
./rt-thread/components/drivers/src/waitqueue.d \
./rt-thread/components/drivers/src/workqueue.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/components/drivers/src/%.o: ../rt-thread/components/drivers/src/%.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\RT-ThreadStudio\workspace\test123\drivers" -I"D:\RT-ThreadStudio\workspace\test123\drivers\include" -I"D:\RT-ThreadStudio\workspace\test123\drivers\include\config" -I"D:\RT-ThreadStudio\workspace\test123\libraries" -I"D:\RT-ThreadStudio\workspace\test123\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\RT-ThreadStudio\workspace\test123\libraries\CMSIS\Include" -I"D:\RT-ThreadStudio\workspace\test123\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\RT-ThreadStudio\workspace\test123" -I"D:\RT-ThreadStudio\workspace\test123\packages\u8g2-c-latest\port" -I"D:\RT-ThreadStudio\workspace\test123\packages\u8g2-c-latest" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\components\drivers\include" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\components\finsh" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\components\libc\compilers\minilibc" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\include" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\libcpu\arm\common" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\libcpu\arm\cortex-m3" -include"D:\RT-ThreadStudio\workspace\test123\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

