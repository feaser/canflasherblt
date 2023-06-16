cmake_minimum_required(VERSION 3.17)

# Configure the toolchain file. Set before the project() line.
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/.cmake/arm-cortex-gcc-toolchain.cmake) 

# Configure C and C++ standards.
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Specify overall project name.
project(apps LANGUAGES C CXX ASM)

# Include the MicroTBX sources.
add_subdirectory(third_party/microtbx)

# Include the software sources.
add_subdirectory(source)


