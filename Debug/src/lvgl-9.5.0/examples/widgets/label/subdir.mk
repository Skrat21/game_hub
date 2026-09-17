################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/examples/widgets/label/lv_example_label_1.c \
../src/lvgl-9.5.0/examples/widgets/label/lv_example_label_2.c \
../src/lvgl-9.5.0/examples/widgets/label/lv_example_label_3.c \
../src/lvgl-9.5.0/examples/widgets/label/lv_example_label_4.c \
../src/lvgl-9.5.0/examples/widgets/label/lv_example_label_5.c \
../src/lvgl-9.5.0/examples/widgets/label/lv_example_label_6.c \
../src/lvgl-9.5.0/examples/widgets/label/lv_example_label_7.c 

OBJS += \
./src/lvgl-9.5.0/examples/widgets/label/lv_example_label_1.o \
./src/lvgl-9.5.0/examples/widgets/label/lv_example_label_2.o \
./src/lvgl-9.5.0/examples/widgets/label/lv_example_label_3.o \
./src/lvgl-9.5.0/examples/widgets/label/lv_example_label_4.o \
./src/lvgl-9.5.0/examples/widgets/label/lv_example_label_5.o \
./src/lvgl-9.5.0/examples/widgets/label/lv_example_label_6.o \
./src/lvgl-9.5.0/examples/widgets/label/lv_example_label_7.o 

C_DEPS += \
./src/lvgl-9.5.0/examples/widgets/label/lv_example_label_1.d \
./src/lvgl-9.5.0/examples/widgets/label/lv_example_label_2.d \
./src/lvgl-9.5.0/examples/widgets/label/lv_example_label_3.d \
./src/lvgl-9.5.0/examples/widgets/label/lv_example_label_4.d \
./src/lvgl-9.5.0/examples/widgets/label/lv_example_label_5.d \
./src/lvgl-9.5.0/examples/widgets/label/lv_example_label_6.d \
./src/lvgl-9.5.0/examples/widgets/label/lv_example_label_7.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/examples/widgets/label/%.o: ../src/lvgl-9.5.0/examples/widgets/label/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


