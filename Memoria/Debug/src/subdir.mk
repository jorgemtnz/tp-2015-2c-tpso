################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Memoria.c \
../src/funcionesConsola.c \
../src/funcionesConstructoras.c \
../src/funciones_Auxiliares.c \
../src/funciones_memoria.c 

OBJS += \
./src/Memoria.o \
./src/funcionesConsola.o \
./src/funcionesConstructoras.o \
./src/funciones_Auxiliares.o \
./src/funciones_memoria.o 

C_DEPS += \
./src/Memoria.d \
./src/funcionesConsola.d \
./src/funcionesConstructoras.d \
./src/funciones_Auxiliares.d \
./src/funciones_memoria.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2015-2c-tpso/utiles" -I"/home/utnso/tp-2015-2c-tpso/commons-library" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


