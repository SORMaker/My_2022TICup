################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/Cpu0_Main.c \
../USER/Cpu1_Main.c \
../USER/isr.c 

OBJS += \
./USER/Cpu0_Main.o \
./USER/Cpu1_Main.o \
./USER/isr.o 

COMPILED_SRCS += \
./USER/Cpu0_Main.src \
./USER/Cpu1_Main.src \
./USER/isr.src 

C_DEPS += \
./USER/Cpu0_Main.d \
./USER/Cpu1_Main.d \
./USER/isr.d 


# Each subdirectory must supply rules for building sources it contributes
USER/%.src: ../USER/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fS:/WorkSpace/Ti����/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

USER/%.o: ./USER/%.src
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


