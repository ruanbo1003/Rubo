################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../msgs/MsgCenter.cpp \
../msgs/UserMsg.cpp 

OBJS += \
./msgs/MsgCenter.o \
./msgs/UserMsg.o 

CPP_DEPS += \
./msgs/MsgCenter.d \
./msgs/UserMsg.d 


# Each subdirectory must supply rules for building sources it contributes
msgs/%.o: ../msgs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=gnu++11 -DDEBUG_MODE=1 -I/usr/include/mysql -I"/home/ruanbo/Codes/Rubo" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


