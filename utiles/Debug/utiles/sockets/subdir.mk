################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utiles/sockets/sockets.c 

OBJS += \
./utiles/sockets/sockets.o 

C_DEPS += \
./utiles/sockets/sockets.d 


# Each subdirectory must supply rules for building sources it contributes
utiles/sockets/%.o: ../utiles/sockets/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2015-2c-tpso/commons-library" -I"/home/utnso/tp-2015-2c-tpso/commons-library/Debug" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


