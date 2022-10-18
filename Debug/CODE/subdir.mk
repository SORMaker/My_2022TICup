################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CODE/camera.c \
../CODE/menu.c \
../CODE/motor.c \
../CODE/pid.c \
../CODE/sendware.c 

OBJS += \
./CODE/camera.o \
./CODE/menu.o \
./CODE/motor.o \
./CODE/pid.o \
./CODE/sendware.o 

COMPILED_SRCS += \
./CODE/camera.src \
./CODE/menu.src \
./CODE/motor.src \
./CODE/pid.src \
./CODE/sendware.src 

C_DEPS += \
./CODE/camera.d \
./CODE/menu.d \
./CODE/motor.d \
./CODE/pid.d \
./CODE/sendware.d 


# Each subdirectory must supply rules for building sources it contributes
CODE/%.src: ../CODE/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fS:/WorkSpace/TiÍêÈü/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

CODE/%.o: ./CODE/%.src
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


