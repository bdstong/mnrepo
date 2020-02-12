################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_f32.c \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_q15.c \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_q31.c \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_f32.c \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_q15.c \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_q31.c \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_f32.c \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_q15.c \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_q31.c \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_f32.c \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_q15.c \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_q31.c \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.c \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.c \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.c \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_f32.c \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_q15.c \
../DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_q31.c 

OBJS += \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_f32.o \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_q15.o \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_q31.o \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_f32.o \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_q15.o \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_q31.o \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_f32.o \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_q15.o \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_q31.o \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_f32.o \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_q15.o \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_q31.o \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.o \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.o \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.o \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_f32.o \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_q15.o \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_q31.o 

C_DEPS += \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_f32.d \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_q15.d \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_q31.d \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_f32.d \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_q15.d \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_q31.d \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_f32.d \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_q15.d \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_q31.d \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_f32.d \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_q15.d \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_q31.d \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.d \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.d \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.d \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_f32.d \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_q15.d \
./DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_q31.d 


# Each subdirectory must supply rules for building sources it contributes
DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_f32.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_f32.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_q15.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_q15.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_q15.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_q31.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_q31.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_conj_q31.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_f32.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_f32.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_q15.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_q15.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_q15.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_q31.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_q31.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_dot_prod_q31.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_f32.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_f32.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_q15.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_q15.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_q15.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_q31.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_q31.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_q31.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_f32.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_f32.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_q15.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_q15.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_q15.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_q31.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_q31.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_mag_squared_q31.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_f32.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_f32.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_q15.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_q15.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_q15.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_q31.o: ../DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_q31.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx '-D__FPU_USED=1U' -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../UserApps/Inc -I../DspLib/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"DspLib/Src/ComplexMathFunctions/arm_cmplx_mult_real_q31.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

