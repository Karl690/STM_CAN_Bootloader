################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cmsis_boot/system_stm32f10x.c 

OBJS += \
./cmsis_boot/system_stm32f10x.o 

C_DEPS += \
./cmsis_boot/system_stm32f10x.d 


# Each subdirectory must supply rules for building sources it contributes
cmsis_boot/%.o cmsis_boot/%.su: ../cmsis_boot/%.c cmsis_boot/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -DSTM32F10X_LD -DOPTIMIZE -c -I../Inc -I"F:/HYREL/GIT/STM_CAN_Bootloader/MedusaBootLoader/cmsis_boot" -I"F:/HYREL/GIT/STM_CAN_Bootloader/MedusaBootLoader/cmsis" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-cmsis_boot

clean-cmsis_boot:
	-$(RM) ./cmsis_boot/system_stm32f10x.d ./cmsis_boot/system_stm32f10x.o ./cmsis_boot/system_stm32f10x.su

.PHONY: clean-cmsis_boot

