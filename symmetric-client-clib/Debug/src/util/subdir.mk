################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/util/List.c \
../src/util/Map.c \
../src/util/Properties.c \
../src/util/StringArray.c \
../src/util/StringBuilder.c 

OBJS += \
./src/util/List.o \
./src/util/Map.o \
./src/util/Properties.o \
./src/util/StringArray.o \
./src/util/StringBuilder.o 

C_DEPS += \
./src/util/List.d \
./src/util/Map.d \
./src/util/Properties.d \
./src/util/StringArray.d \
./src/util/StringBuilder.d 


# Each subdirectory must supply rules for building sources it contributes
src/util/%.o: ../src/util/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/elong/git/3.7/symmetric-ds/symmetric-client-clib/inc" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

