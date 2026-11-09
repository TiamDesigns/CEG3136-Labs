################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Drivers/Src/maths.s 

C_SRCS += \
../Drivers/Src/console.c \
../Drivers/Src/gpio.c \
../Drivers/Src/i2c.c \
../Drivers/Src/syscalls.c \
../Drivers/Src/sysmem.c \
../Drivers/Src/systick.c \
../Drivers/Src/touchpad.c 

OBJS += \
./Drivers/Src/console.o \
./Drivers/Src/gpio.o \
./Drivers/Src/i2c.o \
./Drivers/Src/maths.o \
./Drivers/Src/syscalls.o \
./Drivers/Src/sysmem.o \
./Drivers/Src/systick.o \
./Drivers/Src/touchpad.o 

S_DEPS += \
./Drivers/Src/maths.d 

C_DEPS += \
./Drivers/Src/console.d \
./Drivers/Src/gpio.d \
./Drivers/Src/i2c.d \
./Drivers/Src/syscalls.d \
./Drivers/Src/sysmem.d \
./Drivers/Src/systick.d \
./Drivers/Src/touchpad.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Src/%.o Drivers/Src/%.su Drivers/Src/%.cyclo: ../Drivers/Src/%.c Drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Device/ST/STM32L5xx/Include -I../Drivers/CMSIS/Include -I../Drivers/Inc -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Src/%.o: ../Drivers/Src/%.s Drivers/Src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m33 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Drivers-2f-Src

clean-Drivers-2f-Src:
	-$(RM) ./Drivers/Src/console.cyclo ./Drivers/Src/console.d ./Drivers/Src/console.o ./Drivers/Src/console.su ./Drivers/Src/gpio.cyclo ./Drivers/Src/gpio.d ./Drivers/Src/gpio.o ./Drivers/Src/gpio.su ./Drivers/Src/i2c.cyclo ./Drivers/Src/i2c.d ./Drivers/Src/i2c.o ./Drivers/Src/i2c.su ./Drivers/Src/maths.d ./Drivers/Src/maths.o ./Drivers/Src/syscalls.cyclo ./Drivers/Src/syscalls.d ./Drivers/Src/syscalls.o ./Drivers/Src/syscalls.su ./Drivers/Src/sysmem.cyclo ./Drivers/Src/sysmem.d ./Drivers/Src/sysmem.o ./Drivers/Src/sysmem.su ./Drivers/Src/systick.cyclo ./Drivers/Src/systick.d ./Drivers/Src/systick.o ./Drivers/Src/systick.su ./Drivers/Src/touchpad.cyclo ./Drivers/Src/touchpad.d ./Drivers/Src/touchpad.o ./Drivers/Src/touchpad.su

.PHONY: clean-Drivers-2f-Src

