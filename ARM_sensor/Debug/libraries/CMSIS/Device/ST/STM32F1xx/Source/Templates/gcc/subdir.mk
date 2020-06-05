################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../libraries/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xe.s 

OBJS += \
./libraries/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xe.o 

S_DEPS += \
./libraries/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xe.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/%.o: ../libraries/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/%.s
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -x assembler-with-cpp -I"D:\RT-ThreadStudio\workspace\ARM_sensor" -Xassembler -mimplicit-it=thumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

