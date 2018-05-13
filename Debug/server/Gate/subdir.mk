################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../server/Gate/Gate.cpp \
../server/Gate/GateIO.cpp \
../server/Gate/GateMngr.cpp \
../server/Gate/GateWorker.cpp \
../server/Gate/LogicNode.cpp 

OBJS += \
./server/Gate/Gate.o \
./server/Gate/GateIO.o \
./server/Gate/GateMngr.o \
./server/Gate/GateWorker.o \
./server/Gate/LogicNode.o 

CPP_DEPS += \
./server/Gate/Gate.d \
./server/Gate/GateIO.d \
./server/Gate/GateMngr.d \
./server/Gate/GateWorker.d \
./server/Gate/LogicNode.d 


# Each subdirectory must supply rules for building sources it contributes
server/Gate/%.o: ../server/Gate/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=gnu++11 -DDEBUG_MODE=1 -I/usr/include/mysql -I"/home/ruanbo/Codes/Rubo" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


