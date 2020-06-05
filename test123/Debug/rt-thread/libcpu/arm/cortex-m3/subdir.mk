################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/libcpu/arm/cortex-m3/cpuport.c 

S_UPPER_SRCS += \
../rt-thread/libcpu/arm/cortex-m3/context_gcc.S 

OBJS += \
./rt-thread/libcpu/arm/cortex-m3/context_gcc.o \
./rt-thread/libcpu/arm/cortex-m3/cpuport.o 

S_UPPER_DEPS += \
./rt-thread/libcpu/arm/cortex-m3/context_gcc.d 

C_DEPS += \
./rt-thread/libcpu/arm/cortex-m3/cpuport.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/libcpu/arm/cortex-m3/%.o: ../rt-thread/libcpu/arm/cortex-m3/%.S
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -x assembler-with-cpp -I"D:\RT-ThreadStudio\workspace\test123" -Xassembler -mimplicit-it=thumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
rt-thread/libcpu/arm/cortex-m3/%.o: ../rt-thread/libcpu/arm/cortex-m3/%.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\RT-ThreadStudio\workspace\test123\drivers" -I"D:\RT-ThreadStudio\workspace\test123\drivers\include" -I"D:\RT-ThreadStudio\workspace\test123\drivers\include\config" -I"D:\RT-ThreadStudio\workspace\test123\libraries" -I"D:\RT-ThreadStudio\workspace\test123\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\RT-ThreadStudio\workspace\test123\libraries\CMSIS\Include" -I"D:\RT-ThreadStudio\workspace\test123\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\RT-ThreadStudio\workspace\test123" -I"D:\RT-ThreadStudio\workspace\test123\packages\u8g2-c-latest\port" -I"D:\RT-ThreadStudio\workspace\test123\packages\u8g2-c-latest" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\components\drivers\include" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\components\finsh" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\components\libc\compilers\minilibc" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\include" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\libcpu\arm\common" -I"D:\RT-ThreadStudio\workspace\test123\rt-thread\libcpu\arm\cortex-m3" -include"D:\RT-ThreadStudio\workspace\test123\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

