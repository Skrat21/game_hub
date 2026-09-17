################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/examples/widgets/arc/lv_example_arc_1.c \
../src/lvgl-9.5.0/examples/widgets/arc/lv_example_arc_2.c \
../src/lvgl-9.5.0/examples/widgets/arc/lv_example_arc_3.c 

OBJS += \
./src/lvgl-9.5.0/examples/widgets/arc/lv_example_arc_1.o \
./src/lvgl-9.5.0/examples/widgets/arc/lv_example_arc_2.o \
./src/lvgl-9.5.0/examples/widgets/arc/lv_example_arc_3.o 

C_DEPS += \
./src/lvgl-9.5.0/examples/widgets/arc/lv_example_arc_1.d \
./src/lvgl-9.5.0/examples/widgets/arc/lv_example_arc_2.d \
./src/lvgl-9.5.0/examples/widgets/arc/lv_example_arc_3.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/examples/widgets/arc/%.o: ../src/lvgl-9.5.0/examples/widgets/arc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


