cmake_minimum_required(VERSION 3.23)
project(Forge C)

set(CMAKE_C_STANDARD 99)

add_executable(Forge main.c method.c method.h TCP_forge.c TCP_forge.h UDP_forge.c UDP_forge.h ARP_forge.c ARP_forge.h ICMP_forge.c ICMP_forge.h)
