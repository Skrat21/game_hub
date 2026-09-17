################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/src/drivers/wayland/lv_wayland.c \
../src/lvgl-9.5.0/src/drivers/wayland/lv_wl_egl_backend.c \
../src/lvgl-9.5.0/src/drivers/wayland/lv_wl_g2d_backend.c \
../src/lvgl-9.5.0/src/drivers/wayland/lv_wl_keyboard.c \
../src/lvgl-9.5.0/src/drivers/wayland/lv_wl_pointer.c \
../src/lvgl-9.5.0/src/drivers/wayland/lv_wl_seat.c \
../src/lvgl-9.5.0/src/drivers/wayland/lv_wl_shm_backend.c \
../src/lvgl-9.5.0/src/drivers/wayland/lv_wl_touch.c \
../src/lvgl-9.5.0/src/drivers/wayland/lv_wl_window.c \
../src/lvgl-9.5.0/src/drivers/wayland/lv_wl_xdg_shell.c 

OBJS += \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wayland.o \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_egl_backend.o \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_g2d_backend.o \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_keyboard.o \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_pointer.o \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_seat.o \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_shm_backend.o \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_touch.o \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_window.o \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_xdg_shell.o 

C_DEPS += \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wayland.d \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_egl_backend.d \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_g2d_backend.d \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_keyboard.d \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_pointer.d \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_seat.d \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_shm_backend.d \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_touch.d \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_window.d \
./src/lvgl-9.5.0/src/drivers/wayland/lv_wl_xdg_shell.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/src/drivers/wayland/%.o: ../src/lvgl-9.5.0/src/drivers/wayland/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


