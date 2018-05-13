################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../server/DB/MysqlDb.cpp 

OBJS += \
./server/DB/MysqlDb.o 

CPP_DEPS += \
./server/DB/MysqlDb.d 


# Each subdirectory must supply rules for building sources it contributes
server/DB/%.o: ../server/DB/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/mysql -I"/home/ruanbo/Codes/Rubo" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


