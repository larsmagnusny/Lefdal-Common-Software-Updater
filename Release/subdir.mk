################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Downloaded_File.cpp \
../HTML_PARSER.cpp \
../LinkMiner.cpp \
../Main.cpp \
../ZipOperations.cpp 

OBJS += \
./Downloaded_File.o \
./HTML_PARSER.o \
./LinkMiner.o \
./Main.o \
./ZipOperations.o 

CPP_DEPS += \
./Downloaded_File.d \
./HTML_PARSER.d \
./LinkMiner.d \
./Main.d \
./ZipOperations.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

