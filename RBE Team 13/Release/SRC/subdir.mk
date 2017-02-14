################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SRC/ADC.c \
../SRC/DAC.c \
../SRC/Lab1Code.c \
../SRC/Lab2Code.c \
../SRC/SPI.c \
../SRC/USART.c \
../SRC/main.c 

OBJS += \
./SRC/ADC.o \
./SRC/DAC.o \
./SRC/Lab1Code.o \
./SRC/Lab2Code.o \
./SRC/SPI.o \
./SRC/USART.o \
./SRC/main.o 

C_DEPS += \
./SRC/ADC.d \
./SRC/DAC.d \
./SRC/Lab1Code.d \
./SRC/Lab2Code.d \
./SRC/SPI.d \
./SRC/USART.d \
./SRC/main.d 


# Each subdirectory must supply rules for building sources it contributes
SRC/%.o: ../SRC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"D:\workspace\RBELib\include" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=18432000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


