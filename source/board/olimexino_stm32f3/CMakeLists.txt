# Configure the project name.
project(CanFlasherBLT_Olimexino_STM32F3)

# Include board specific third party components.
include("${CMAKE_SOURCE_DIR}/.cmake/CubeF3.cmake")
include("${CMAKE_SOURCE_DIR}/.cmake/TinyUSB.cmake")
include("${CMAKE_SOURCE_DIR}/.cmake/SeggerRTT.cmake")

# Configure project sources.
set(PROJECT_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/startup_stm32f303rctx.s"
    "${CMAKE_CURRENT_LIST_DIR}/system_stm32f3xx.c"
    "${CMAKE_CURRENT_LIST_DIR}/syscalls.c"
    "${CMAKE_CURRENT_LIST_DIR}/sysmem.c"
    "${CMAKE_CURRENT_LIST_DIR}/FreeRTOSHooks.c"
    "${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c"
    "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/rttlogger.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/hardwareboard.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/statusled.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/tinyusbdevice.cpp"
)

# Configure project include paths.
set(PROJECT_INCLUDES
    "${CMAKE_CURRENT_LIST_DIR}"
)

# Configure project libraries.
set(PROJECT_LIBRARIES
    application
    microtbx-cortexm
    cube_f3_ll
    freertos-arm-cm4f 
    tinyusb
    tinyusb_stm32f3
    segger_rtt
)

# Configure project specific compiler definitions.
set(PROJECT_DEFINITIONS STM32F303xC USER_VECT_TAB_ADDRESS)

# Configure project specific CPU/compiler flags and linker script.
set(PROJECT_CPU_FLAGS -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16)
set(PROJECT_COMPILER_FLAGS -Wall -Wextra -Wno-unused-parameter -fmacro-prefix-map=${CMAKE_SOURCE_DIR}/=./)
set(PROJECT_LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/STM32F303RCTX_FLASH.ld")

# Set target executable.
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

# Add additional compiler options.
target_compile_options(${PROJECT_NAME} PRIVATE
    ${PROJECT_CPU_FLAGS}
    ${PROJECT_COMPILER_FLAGS}
)

# Add additional compiler definitions.
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ${PROJECT_DEFINITIONS}
)

# Add include paths.
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_INCLUDES}
)

# Add libraries to include.
target_link_libraries(${PROJECT_NAME}
    ${PROJECT_LIBRARIES}
)

# Add additional link options.
target_link_options(${PROJECT_NAME} PRIVATE
    ${PROJECT_CPU_FLAGS}
    -T "${PROJECT_LINKER_SCRIPT}"
    --specs=nano.specs
)

# Generate a map file (optional).
arm_cortex_gcc_generate_map(${PROJECT_NAME})

# Generate an additional firmware file in the S-record format (optional).
arm_cortex_gcc_generate_extra_output(${PROJECT_NAME} .srec srec)

# Show ROM/RAM usage information at the end of the build (optional).
arm_cortex_gcc_display_size(${PROJECT_NAME})

