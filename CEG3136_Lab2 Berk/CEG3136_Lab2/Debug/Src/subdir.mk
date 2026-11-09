################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Src/math.s 

C_SRCS += \
../Src/alarm.c \
../Src/calc.c \
../Src/main.c 

OBJS += \
./Src/alarm.o \
./Src/calc.o \
./Src/main.o \
./Src/math.o 

S_DEPS += \
./Src/math.d 

C_DEPS += \
./Src/alarm.d \
./Src/calc.d \
./Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Device/ST/STM32L5xx/Include -I../Drivers/CMSIS/Include -I../Drivers/Inc -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/%.o: ../Src/%.s Src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m33 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/alarm.cyclo ./Src/alarm.d ./Src/alarm.o ./Src/alarm.su ./Src/calc.cyclo ./Src/calc.d ./Src/calc.o ./Src/calc.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/math.d ./Src/math.o

.PHONY: clean-Src

