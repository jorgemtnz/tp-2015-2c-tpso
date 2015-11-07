################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utiles/configExtras.c \
../utiles/espacioDeDatos.c \
../utiles/files.c \
../utiles/mapeoAMemoria.c \
../utiles/protocolo.c 

OBJS += \
./utiles/configExtras.o \
./utiles/espacioDeDatos.o \
./utiles/files.o \
./utiles/mapeoAMemoria.o \
./utiles/protocolo.o 

C_DEPS += \
./utiles/configExtras.d \
./utiles/espacioDeDatos.d \
./utiles/files.d \
./utiles/mapeoAMemoria.d \
./utiles/protocolo.d 


# Each subdirectory must supply rules for building sources it contributes
utiles/%.o: ../utiles/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2015-2c-tpso/commons-library" -I"/home/utnso/tp-2015-2c-tpso/commons-library/Debug" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


