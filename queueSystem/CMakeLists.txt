cmake_minimum_required(VERSION 2.8)
project(komunikaty)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp CommunicatesStack.cpp CommunicatesStack.h MachineBlock.cpp MachineBlock.h Communicate.cpp Communicate.h Statistics.cpp Statistics.h DestinationMachine.cpp DestinationMachine.h)
add_executable(komunikaty ${SOURCE_FILES})