cmake_minimum_required(VERSION "3.15")
project(ssedb C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# Register Zydis dependency.
# Disable build of tools and examples.
option(ZYDIS_BUILD_TOOLS "" OFF)
option(ZYDIS_BUILD_EXAMPLES "" OFF)
add_subdirectory("deps/zydis")

# Add our project executable
add_executable(ssedb ast.c cmdFuncs.c main.c)

add_compile_definitions("PACKAGE=ssedb")
add_compile_definitions("PACKAGE_VERSION=1")

# Have CMake link our project executable against Zydis.
target_link_libraries("ssedb" PRIVATE "Zydis" -lbfd -liberty -ldl -lz)
