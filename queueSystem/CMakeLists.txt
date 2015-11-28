cmake_minimum_required(VERSION 3.3)
project(komunikaty)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp CommunicatesStack.cpp CommunicatesStack.h MachineBlock.cpp MachineBlock.h Communicate.cpp Communicate.h)
add_executable(komunikaty ${SOURCE_FILES})