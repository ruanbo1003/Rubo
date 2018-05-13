################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../server/ServerBase.cpp \
../server/ServerMain.cpp 

OBJS += \
./server/ServerBase.o \
./server/ServerMain.o 

CPP_DEPS += \
./server/ServerBase.d \
./server/ServerMain.d 


# Each subdirectory must supply rules for building sources it contributes
server/%.o: ../server/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/mysql -I"/home/ruanbo/Codes/Rubo" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


