################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data.cpp \
../src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_animations.cpp \
../src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_cache.cpp \
../src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_injest.cpp \
../src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_mesh.cpp \
../src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_primitive.cpp \
../src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_shader.cpp \
../src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_skin.cpp \
../src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_texture.cpp \
../src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_model_node.cpp \
../src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_uniform_locations.cpp 

OBJS += \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data.o \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_animations.o \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_cache.o \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_injest.o \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_mesh.o \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_primitive.o \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_shader.o \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_skin.o \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_texture.o \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_model_node.o \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_uniform_locations.o 

CPP_DEPS += \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data.d \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_animations.d \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_cache.d \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_injest.d \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_mesh.d \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_primitive.d \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_shader.d \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_skin.d \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_data_texture.d \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_model_node.d \
./src/lvgl-9.5.0/src/libs/gltf/gltf_data/lv_gltf_uniform_locations.d 


# Each subdirectory must supply rules for building sources it contributes
src/lvgl-9.5.0/src/libs/gltf/gltf_data/%.o: ../src/lvgl-9.5.0/src/libs/gltf/gltf_data/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=16000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


