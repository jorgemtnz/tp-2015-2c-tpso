################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Test/test.c \
../Test/test_memoria.c 

OBJS += \
./Test/test.o \
./Test/test_memoria.o 

C_DEPS += \
./Test/test.d \
./Test/test_memoria.d 


# Each subdirectory must supply rules for building sources it contributes
Test/%.o: ../Test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2015-2c-tpsobranch/utiles" -I"/home/utnso/tp-2015-2c-tpsobranch/commons-library" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


