# Create interface library for the Segger Real-time Target (RTT) sources.
add_library(segger_rtt INTERFACE)

target_sources(segger_rtt INTERFACE
    "${CMAKE_SOURCE_DIR}/third_party/segger/rtt/RTT/SEGGER_RTT.c"
    "${CMAKE_SOURCE_DIR}/third_party/segger/rtt/RTT/SEGGER_RTT_printf.c"
    "${CMAKE_SOURCE_DIR}/third_party/segger/rtt/Syscalls/SEGGER_RTT_Syscalls_GCC.c"
)

target_include_directories(segger_rtt INTERFACE 
    "${CMAKE_SOURCE_DIR}/third_party/segger/rtt/Config"
    "${CMAKE_SOURCE_DIR}/third_party/segger/rtt/RTT"
)

# Use a smaller buffer for Segger's printf to lower the stack load. Beneficial when
# it might be called from multiple tasks in an RTOS, because each tasks has it's own
# stack.
target_compile_definitions(segger_rtt INTERFACE
    SEGGER_RTT_PRINTF_BUFFER_SIZE=16
)

