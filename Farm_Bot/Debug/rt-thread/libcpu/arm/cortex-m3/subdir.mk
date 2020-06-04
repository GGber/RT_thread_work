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
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -x assembler-with-cpp -I"D:\RT-ThreadStudio\workspace\Farm013" -Xassembler -mimplicit-it=thumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

rt-thread/libcpu/arm/cortex-m3/%.o: ../rt-thread/libcpu/arm/cortex-m3/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\RT-ThreadStudio\workspace\Farm013\drivers" -I"D:\RT-ThreadStudio\workspace\Farm013\libraries" -I"D:\RT-ThreadStudio\workspace\Farm013\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\RT-ThreadStudio\workspace\Farm013\libraries\CMSIS\Include" -I"D:\RT-ThreadStudio\workspace\Farm013\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\RT-ThreadStudio\workspace\Farm013" -I"D:\RT-ThreadStudio\workspace\Farm013\packages\freemodbus-latest\modbus\ascii" -I"D:\RT-ThreadStudio\workspace\Farm013\packages\freemodbus-latest\modbus\include" -I"D:\RT-ThreadStudio\workspace\Farm013\packages\freemodbus-latest\modbus\rtu" -I"D:\RT-ThreadStudio\workspace\Farm013\packages\freemodbus-latest\port" -I"D:\RT-ThreadStudio\workspace\Farm013\rt-thread\components\drivers\include" -I"D:\RT-ThreadStudio\workspace\Farm013\rt-thread\components\finsh" -I"D:\RT-ThreadStudio\workspace\Farm013\rt-thread\components\libc\compilers\minilibc" -I"D:\RT-ThreadStudio\workspace\Farm013\rt-thread\include" -I"D:\RT-ThreadStudio\workspace\Farm013\rt-thread\libcpu\arm\common" -I"D:\RT-ThreadStudio\workspace\Farm013\rt-thread\libcpu\arm\cortex-m3" -include"D:\RT-ThreadStudio\workspace\Farm013\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


