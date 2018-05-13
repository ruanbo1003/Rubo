################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../comm/Log.cpp \
../comm/comm.cpp \
../comm/global.cpp 

OBJS += \
./comm/Log.o \
./comm/comm.o \
./comm/global.o 

CPP_DEPS += \
./comm/Log.d \
./comm/comm.d \
./comm/global.d 


# Each subdirectory must supply rules for building sources it contributes
comm/%.o: ../comm/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/mysql -I"/home/ruanbo/Codes/Rubo" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


