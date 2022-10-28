cmake_minimum_required(VERSION 3.23)
project(final C)

set(CMAKE_C_STANDARD 99)

add_executable(final main.c send.c protocol.h receive.c)
