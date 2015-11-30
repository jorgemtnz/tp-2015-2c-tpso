################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utiles/subdir.mk
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
all: libutiles.so

dependents:
	-cd /home/utnso/tp-2015-2c-tpsobranch/commons-library/Debug && $(MAKE) all

# Tool invocations
libutiles.so: $(OBJS) $(USER_OBJS) /home/utnso/tp-2015-2c-tpsobranch/commons-library/Debug/libcommons-library.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/tp-2015-2c-tpsobranch/commons-library/Debug" -shared -o "libutiles.so" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(LIBRARIES) libutiles.so
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/tp-2015-2c-tpsobranch/commons-library/Debug/libcommons-library.so:

-include ../makefile.targets
