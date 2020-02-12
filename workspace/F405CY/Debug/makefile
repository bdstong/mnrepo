################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include UserApps/Src/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include DspLib/Src/TransformFunctions/subdir.mk
-include DspLib/Src/SupportFunctions/subdir.mk
-include DspLib/Src/StatisticsFunctions/subdir.mk
-include DspLib/Src/MatrixFunctions/subdir.mk
-include DspLib/Src/FilteringFunctions/subdir.mk
-include DspLib/Src/FastMathFunctions/subdir.mk
-include DspLib/Src/ControllerFunctions/subdir.mk
-include DspLib/Src/ComplexMathFunctions/subdir.mk
-include DspLib/Src/CommonTables/subdir.mk
-include DspLib/Src/BasicMathFunctions/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
F405CY.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
F405CY.list \

OBJCOPY_HEX += \
F405CY.hex \

OBJCOPY_BIN += \
F405CY.bin \


# All Target
all: F405CY.elf secondary-outputs

# Tool invocations
F405CY.elf: $(OBJS) $(USER_OBJS) D:\STM32CubeIDE\workspace\F405CY\STM32F405RGTX_FLASH.ld
	arm-none-eabi-gcc -o "F405CY.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\STM32CubeIDE\workspace\F405CY\STM32F405RGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="F405CY.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

F405CY.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "F405CY.list"
	@echo 'Finished building: $@'
	@echo ' '

F405CY.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "F405CY.hex"
	@echo 'Finished building: $@'
	@echo ' '

F405CY.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "F405CY.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
