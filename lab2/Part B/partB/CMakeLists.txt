cmake_minimum_required(VERSION 3.16)
project(miniWireShark C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(miniWireShark main.c packetProcess.h packetProcess.c)
