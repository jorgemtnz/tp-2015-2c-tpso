################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include test/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Swap

dependents:
	-cd /home/utnso/tp-2015-2c-tpsobranch/commons-library/Debug && $(MAKE) clean all
	-cd /home/utnso/tp-2015-2c-tpsobranch/utiles/Debug && $(MAKE) clean all

# Tool invocations
Swap: $(OBJS) $(USER_OBJS) /home/utnso/tp-2015-2c-tpsobranch/commons-library/Debug/libcommons-library.so /home/utnso/tp-2015-2c-tpsobranch/utiles/Debug/libutiles.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/tp-2015-2c-tpsobranch/utiles/Debug" -L"/home/utnso/tp-2015-2c-tpsobranch/commons-library/Debug" -o "Swap" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) Swap
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/tp-2015-2c-tpsobranch/commons-library/Debug/libcommons-library.so:
/home/utnso/tp-2015-2c-tpsobranch/utiles/Debug/libutiles.so:

-include ../makefile.targets
