################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_cbfs.c \
../src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_fatfs.c \
../src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_frogfs.c \
../src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_littlefs.c \
../src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_memfs.c \
../src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_posix.c \
../src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_stdio.c \
../src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_uefi.c \
../src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_win32.c 

CPP_SRCS += \
../src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_arduino_esp_littlefs.cpp \
../src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_arduino_sd.cpp 

OBJS += \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_arduino_esp_littlefs.o \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_arduino_sd.o \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_cbfs.o \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_fatfs.o \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_frogfs.o \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_littlefs.o \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_memfs.o \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_posix.o \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_stdio.o \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_uefi.o \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_win32.o 

C_DEPS += \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_cbfs.d \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_fatfs.d \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_frogfs.d \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_littlefs.d \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_memfs.d \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_posix.d \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_stdio.d \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_uefi.d \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_win32.d 

CPP_DEPS += \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_arduino_esp_littlefs.d \
./src/lvgl-9.5.0/src/libs/fsdrv/lv_fs_arduino_sd.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/src/libs/fsdrv/%.o: ../src/lvgl-9.5.0/src/libs/fsdrv/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=16000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/lvgl-9.5.0/src/libs/fsdrv/%.o: ../src/lvgl-9.5.0/src/libs/fsdrv/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


