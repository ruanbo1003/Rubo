################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../msgs/protocols/login.pb.cc 

CC_DEPS += \
./msgs/protocols/login.pb.d 

OBJS += \
./msgs/protocols/login.pb.o 


# Each subdirectory must supply rules for building sources it contributes
msgs/protocols/%.o: ../msgs/protocols/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/mysql -I"/home/ruanbo/Codes/Rubo" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


