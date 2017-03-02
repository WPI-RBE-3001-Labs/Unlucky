################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SRC/ADC.c \
../SRC/DAC.c \
../SRC/Final.c \
../SRC/Kinematics.c \
../SRC/Lab1Code.c \
../SRC/Lab2Code.c \
../SRC/Lab3Stuff.c \
../SRC/PID.c \
../SRC/SPI.c \
../SRC/USART.c \
../SRC/main.c \
../SRC/timer.c 

OBJS += \
./SRC/ADC.o \
./SRC/DAC.o \
./SRC/Final.o \
./SRC/Kinematics.o \
./SRC/Lab1Code.o \
./SRC/Lab2Code.o \
./SRC/Lab3Stuff.o \
./SRC/PID.o \
./SRC/SPI.o \
./SRC/USART.o \
./SRC/main.o \
./SRC/timer.o 

C_DEPS += \
./SRC/ADC.d \
./SRC/DAC.d \
./SRC/Final.d \
./SRC/Kinematics.d \
./SRC/Lab1Code.d \
./SRC/Lab2Code.d \
./SRC/Lab3Stuff.d \
./SRC/PID.d \
./SRC/SPI.d \
./SRC/USART.d \
./SRC/main.d \
./SRC/timer.d 


# Each subdirectory must supply rules for building sources it contributes
SRC/%.o: ../SRC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"D:\workspace\RBELib\include" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=18432000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


