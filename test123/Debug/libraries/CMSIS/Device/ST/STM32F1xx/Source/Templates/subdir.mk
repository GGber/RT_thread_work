################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c 

OBJS += \
./libraries/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.o 

C_DEPS += \
./libraries/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/CMSIS/Device/ST/STM32F1xx/Source/Templates/%.o: ../libraries/CMSIS/Device/ST/STM32F1xx/Source/Templates/%.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\RT-ThreadStudio\workspace\test123\drivers" -I"D:\RT-ThreadStudio\workspace\test123\drivers\include" -I"D:\RT-ThreadStudio\workspace\test123\drivers\include\config" -I"D:\RT-ThreadStudio\workspace\test123\libraries" -I"D:\RT-ThreadStudio\workspace\test123\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\RT-ThreadStudio\workspace\test123\libraries\CMSIS\Include" -I"D:\RT-ThreadStudio\workspace\test123\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\RT-ThreadStudio\workspace\test123" -I"D:\RT-ThreadStudio\workspace\test123\packages\u8g2-c-latest\port" -I"D:\RT-ThreadStudio\workspace\test123\packages\u8g2-c-latest" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\components\drivers\include" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\components\finsh" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\components\libc\compilers\minilibc" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\include" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\libcpu\arm\common" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\libcpu\arm\cortex-m3" -include"D:\RT-ThreadStudio\workspace\test123\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

