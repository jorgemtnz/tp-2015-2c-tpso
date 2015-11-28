################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Swap.c \
../src/constructoras.c \
../src/funciones_swap.c 

OBJS += \
./src/Swap.o \
./src/constructoras.o \
./src/funciones_swap.o 

C_DEPS += \
./src/Swap.d \
./src/constructoras.d \
./src/funciones_swap.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2015-2c-tpsobranch/utiles" -I"/home/utnso/tp-2015-2c-tpsobranch/commons-library" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


