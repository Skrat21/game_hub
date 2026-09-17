################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_1.c \
../src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_2.c \
../src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_3.c \
../src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_4.c \
../src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_5.c \
../src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_6.c 

OBJS += \
./src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_1.o \
./src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_2.o \
./src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_3.o \
./src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_4.o \
./src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_5.o \
./src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_6.o 

C_DEPS += \
./src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_1.d \
./src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_2.d \
./src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_3.d \
./src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_4.d \
./src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_5.d \
./src/lvgl-9.5.0/examples/layouts/grid/lv_example_grid_6.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/examples/layouts/grid/%.o: ../src/lvgl-9.5.0/examples/layouts/grid/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


