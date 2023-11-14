################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/mySrc/Callbacks.c \
../Core/Src/mySrc/Communication.c \
../Core/Src/mySrc/Feedback.c \
../Core/Src/mySrc/MotorWriters.c \
../Core/Src/mySrc/myMain.c 

OBJS += \
./Core/Src/mySrc/Callbacks.o \
./Core/Src/mySrc/Communication.o \
./Core/Src/mySrc/Feedback.o \
./Core/Src/mySrc/MotorWriters.o \
./Core/Src/mySrc/myMain.o 

C_DEPS += \
./Core/Src/mySrc/Callbacks.d \
./Core/Src/mySrc/Communication.d \
./Core/Src/mySrc/Feedback.d \
./Core/Src/mySrc/MotorWriters.d \
./Core/Src/mySrc/myMain.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/mySrc/%.o Core/Src/mySrc/%.su Core/Src/mySrc/%.cyclo: ../Core/Src/mySrc/%.c Core/Src/mySrc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-mySrc

clean-Core-2f-Src-2f-mySrc:
	-$(RM) ./Core/Src/mySrc/Callbacks.cyclo ./Core/Src/mySrc/Callbacks.d ./Core/Src/mySrc/Callbacks.o ./Core/Src/mySrc/Callbacks.su ./Core/Src/mySrc/Communication.cyclo ./Core/Src/mySrc/Communication.d ./Core/Src/mySrc/Communication.o ./Core/Src/mySrc/Communication.su ./Core/Src/mySrc/Feedback.cyclo ./Core/Src/mySrc/Feedback.d ./Core/Src/mySrc/Feedback.o ./Core/Src/mySrc/Feedback.su ./Core/Src/mySrc/MotorWriters.cyclo ./Core/Src/mySrc/MotorWriters.d ./Core/Src/mySrc/MotorWriters.o ./Core/Src/mySrc/MotorWriters.su ./Core/Src/mySrc/myMain.cyclo ./Core/Src/mySrc/myMain.d ./Core/Src/mySrc/myMain.o ./Core/Src/mySrc/myMain.su

.PHONY: clean-Core-2f-Src-2f-mySrc

