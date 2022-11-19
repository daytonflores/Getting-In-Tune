################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/adc.c \
../source/autocorrelate.c \
../source/dac.c \
../source/dma.c \
../source/main.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/systick.c \
../source/test_sine.c 

O_SRCS += \
../source/fp_trig.o 

C_DEPS += \
./source/adc.d \
./source/autocorrelate.d \
./source/dac.d \
./source/dma.d \
./source/main.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/systick.d \
./source/test_sine.d 

OBJS += \
./source/adc.o \
./source/autocorrelate.o \
./source/dac.o \
./source/dma.o \
./source/main.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/systick.o \
./source/test_sine.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DPRINTF_FLOAT_ENABLE=1 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\GettingInTune\board" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\GettingInTune\source" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\GettingInTune" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\GettingInTune\drivers" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\GettingInTune\CMSIS" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\GettingInTune\utilities" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\GettingInTune\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/adc.d ./source/adc.o ./source/autocorrelate.d ./source/autocorrelate.o ./source/dac.d ./source/dac.o ./source/dma.d ./source/dma.o ./source/main.d ./source/main.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/systick.d ./source/systick.o ./source/test_sine.d ./source/test_sine.o

.PHONY: clean-source

