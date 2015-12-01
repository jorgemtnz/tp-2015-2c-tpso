################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Planificador.c \
../src/comandos.c \
../src/funcionesConsola.c \
../src/funciones_plan.c \
../src/planificacion.c 

OBJS += \
./src/Planificador.o \
./src/comandos.o \
./src/funcionesConsola.o \
./src/funciones_plan.o \
./src/planificacion.o 

C_DEPS += \
./src/Planificador.d \
./src/comandos.d \
./src/funcionesConsola.d \
./src/funciones_plan.d \
./src/planificacion.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2015-2c-tpso/utiles" -I"/home/utnso/tp-2015-2c-tpso/commons-library" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


