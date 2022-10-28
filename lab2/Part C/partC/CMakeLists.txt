cmake_minimum_required(VERSION 3.16)
project(partC_analysis C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(.)

add_executable(partC_analysis
        filter.c
        filter.h
        main.c
        packetProcess.c
        packetProcess.h)
