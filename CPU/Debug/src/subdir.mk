################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CPU.c \
../src/const_destr.c \
../src/ejecutarComandos.c \
../src/envioYrecepcion.c \
../src/funciones_auxiliares.c \
../src/funciones_cpu.c \
../src/hilosCPU.c 

OBJS += \
./src/CPU.o \
./src/const_destr.o \
./src/ejecutarComandos.o \
./src/envioYrecepcion.o \
./src/funciones_auxiliares.o \
./src/funciones_cpu.o \
./src/hilosCPU.o 

C_DEPS += \
./src/CPU.d \
./src/const_destr.d \
./src/ejecutarComandos.d \
./src/envioYrecepcion.d \
./src/funciones_auxiliares.d \
./src/funciones_cpu.d \
./src/hilosCPU.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2015-2c-tpso/utiles" -I"/home/utnso/tp-2015-2c-tpso/commons-library" -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


