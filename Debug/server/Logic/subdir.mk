################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../server/Logic/Logic.cpp \
../server/Logic/LogicIO.cpp \
../server/Logic/LogicMngr.cpp \
../server/Logic/LogicUser.cpp \
../server/Logic/LogicWorker.cpp 

OBJS += \
./server/Logic/Logic.o \
./server/Logic/LogicIO.o \
./server/Logic/LogicMngr.o \
./server/Logic/LogicUser.o \
./server/Logic/LogicWorker.o 

CPP_DEPS += \
./server/Logic/Logic.d \
./server/Logic/LogicIO.d \
./server/Logic/LogicMngr.d \
./server/Logic/LogicUser.d \
./server/Logic/LogicWorker.d 


# Each subdirectory must supply rules for building sources it contributes
server/Logic/%.o: ../server/Logic/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=gnu++11 -DDEBUG_MODE=1 -I/usr/include/mysql -I"/home/ruanbo/Codes/Rubo" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


