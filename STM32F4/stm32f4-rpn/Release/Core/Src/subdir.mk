################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/input.cpp \
../Core/Src/lexer.cpp \
../Core/Src/main.cpp \
../Core/Src/object.cpp \
../Core/Src/program.cpp \
../Core/Src/real-out.cpp \
../Core/Src/rpn-branch.cpp \
../Core/Src/rpn-complex.cpp \
../Core/Src/rpn-general.cpp \
../Core/Src/rpn-logs.cpp \
../Core/Src/rpn-program.cpp \
../Core/Src/rpn-real.cpp \
../Core/Src/rpn-stack.cpp \
../Core/Src/rpn-store.cpp \
../Core/Src/rpn-string.cpp \
../Core/Src/rpn-test-framework.cpp \
../Core/Src/rpn-test.cpp \
../Core/Src/rpn-time.cpp \
../Core/Src/rpn-trig.cpp 

C_SRCS += \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

C_DEPS += \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 

OBJS += \
./Core/Src/input.o \
./Core/Src/lexer.o \
./Core/Src/main.o \
./Core/Src/object.o \
./Core/Src/program.o \
./Core/Src/real-out.o \
./Core/Src/rpn-branch.o \
./Core/Src/rpn-complex.o \
./Core/Src/rpn-general.o \
./Core/Src/rpn-logs.o \
./Core/Src/rpn-program.o \
./Core/Src/rpn-real.o \
./Core/Src/rpn-stack.o \
./Core/Src/rpn-store.o \
./Core/Src/rpn-string.o \
./Core/Src/rpn-test-framework.o \
./Core/Src/rpn-test.o \
./Core/Src/rpn-time.o \
./Core/Src/rpn-trig.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

CPP_DEPS += \
./Core/Src/input.d \
./Core/Src/lexer.d \
./Core/Src/main.d \
./Core/Src/object.d \
./Core/Src/program.d \
./Core/Src/real-out.d \
./Core/Src/rpn-branch.d \
./Core/Src/rpn-complex.d \
./Core/Src/rpn-general.d \
./Core/Src/rpn-logs.d \
./Core/Src/rpn-program.d \
./Core/Src/rpn-real.d \
./Core/Src/rpn-stack.d \
./Core/Src/rpn-store.d \
./Core/Src/rpn-string.d \
./Core/Src/rpn-test-framework.d \
./Core/Src/rpn-test.d \
./Core/Src/rpn-time.d \
./Core/Src/rpn-trig.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++20 -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu18 -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/input.cyclo ./Core/Src/input.d ./Core/Src/input.o ./Core/Src/input.su ./Core/Src/lexer.cyclo ./Core/Src/lexer.d ./Core/Src/lexer.o ./Core/Src/lexer.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/object.cyclo ./Core/Src/object.d ./Core/Src/object.o ./Core/Src/object.su ./Core/Src/program.cyclo ./Core/Src/program.d ./Core/Src/program.o ./Core/Src/program.su ./Core/Src/real-out.cyclo ./Core/Src/real-out.d ./Core/Src/real-out.o ./Core/Src/real-out.su ./Core/Src/rpn-branch.cyclo ./Core/Src/rpn-branch.d ./Core/Src/rpn-branch.o ./Core/Src/rpn-branch.su ./Core/Src/rpn-complex.cyclo ./Core/Src/rpn-complex.d ./Core/Src/rpn-complex.o ./Core/Src/rpn-complex.su ./Core/Src/rpn-general.cyclo ./Core/Src/rpn-general.d ./Core/Src/rpn-general.o ./Core/Src/rpn-general.su ./Core/Src/rpn-logs.cyclo ./Core/Src/rpn-logs.d ./Core/Src/rpn-logs.o ./Core/Src/rpn-logs.su ./Core/Src/rpn-program.cyclo ./Core/Src/rpn-program.d ./Core/Src/rpn-program.o ./Core/Src/rpn-program.su ./Core/Src/rpn-real.cyclo ./Core/Src/rpn-real.d ./Core/Src/rpn-real.o ./Core/Src/rpn-real.su ./Core/Src/rpn-stack.cyclo ./Core/Src/rpn-stack.d ./Core/Src/rpn-stack.o ./Core/Src/rpn-stack.su ./Core/Src/rpn-store.cyclo ./Core/Src/rpn-store.d ./Core/Src/rpn-store.o ./Core/Src/rpn-store.su ./Core/Src/rpn-string.cyclo ./Core/Src/rpn-string.d ./Core/Src/rpn-string.o ./Core/Src/rpn-string.su ./Core/Src/rpn-test-framework.cyclo ./Core/Src/rpn-test-framework.d ./Core/Src/rpn-test-framework.o ./Core/Src/rpn-test-framework.su ./Core/Src/rpn-test.cyclo ./Core/Src/rpn-test.d ./Core/Src/rpn-test.o ./Core/Src/rpn-test.su ./Core/Src/rpn-time.cyclo ./Core/Src/rpn-time.d ./Core/Src/rpn-time.o ./Core/Src/rpn-time.su ./Core/Src/rpn-trig.cyclo ./Core/Src/rpn-trig.d ./Core/Src/rpn-trig.o ./Core/Src/rpn-trig.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

