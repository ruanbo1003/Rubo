################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tests/db_test.cpp \
../tests/json_test.cpp \
../tests/msg_test.cpp \
../tests/redis_test.cpp \
../tests/ringbuff_test.cpp \
../tests/server_test.cpp \
../tests/smart_ptr.cpp \
../tests/template_tests.cpp \
../tests/threads_test.cpp \
../tests/time_test.cpp 

OBJS += \
./tests/db_test.o \
./tests/json_test.o \
./tests/msg_test.o \
./tests/redis_test.o \
./tests/ringbuff_test.o \
./tests/server_test.o \
./tests/smart_ptr.o \
./tests/template_tests.o \
./tests/threads_test.o \
./tests/time_test.o 

CPP_DEPS += \
./tests/db_test.d \
./tests/json_test.d \
./tests/msg_test.d \
./tests/redis_test.d \
./tests/ringbuff_test.d \
./tests/server_test.d \
./tests/smart_ptr.d \
./tests/template_tests.d \
./tests/threads_test.d \
./tests/time_test.d 


# Each subdirectory must supply rules for building sources it contributes
tests/%.o: ../tests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/mysql -I"/home/ruanbo/Codes/Rubo" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


