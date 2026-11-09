################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/alarm.c \
../Src/calc.c \
../Src/display.c \
../Src/main.c 

OBJS += \
./Src/alarm.o \
./Src/calc.o \
./Src/display.o \
./Src/main.o 

C_DEPS += \
./Src/alarm.d \
./Src/calc.d \
./Src/display.d \
./Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Device/ST/STM32L5xx/Include -I../Drivers/CMSIS/Include -I../Drivers/Inc -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/alarm.cyclo ./Src/alarm.d ./Src/alarm.o ./Src/alarm.su ./Src/calc.cyclo ./Src/calc.d ./Src/calc.o ./Src/calc.su ./Src/display.cyclo ./Src/display.d ./Src/display.o ./Src/display.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su

.PHONY: clean-Src

