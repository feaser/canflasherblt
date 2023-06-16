# Create interface library for the hardware independent FreeRTOS sources.
add_library(freertos INTERFACE)

target_sources(freertos INTERFACE
    "${CMAKE_SOURCE_DIR}/third_party/freertos/croutine.c"
    "${CMAKE_SOURCE_DIR}/third_party/freertos/event_groups.c"
    "${CMAKE_SOURCE_DIR}/third_party/freertos/list.c"
    "${CMAKE_SOURCE_DIR}/third_party/freertos/queue.c"
    "${CMAKE_SOURCE_DIR}/third_party/freertos/stream_buffer.c"
    "${CMAKE_SOURCE_DIR}/third_party/freertos/tasks.c"
    "${CMAKE_SOURCE_DIR}/third_party/freertos/timers.c"
)

target_include_directories(freertos INTERFACE
    "${CMAKE_SOURCE_DIR}/third_party/freertos/include"
)

# Create interface library for the ARM Cortex-M4 port sources.
add_library(freertos-arm-cm4f INTERFACE)

target_sources(freertos-arm-cm4f INTERFACE
    "${CMAKE_SOURCE_DIR}/third_party/freertos/portable/GCC/ARM_CM4F/port.c"
)

target_include_directories(freertos-arm-cm4f INTERFACE
    "${CMAKE_SOURCE_DIR}/third_party/freertos/portable/GCC/ARM_CM4F"
)

# Create interface library for the Heap 1 memory manager sources.
add_library(freertos-heap1 INTERFACE)

target_sources(freertos-heap1 INTERFACE
    "${CMAKE_SOURCE_DIR}/third_party/freertos/portable/MemMang/heap_1.c"
)

# Create interface library for the Heap 2 memory manager sources.
add_library(freertos-heap2 INTERFACE)

target_sources(freertos-heap2 INTERFACE
    "${CMAKE_SOURCE_DIR}/third_party/freertos/portable/MemMang/heap_2.c"
)

# Create interface library for the Heap 3 memory manager sources.
add_library(freertos-heap3 INTERFACE)

target_sources(freertos-heap3 INTERFACE
    "${CMAKE_SOURCE_DIR}/third_party/freertos/portable/MemMang/heap_3.c"
)

# Create interface library for the Heap 4 memory manager sources.
add_library(freertos-heap4 INTERFACE)

target_sources(freertos-heap4 INTERFACE
    "${CMAKE_SOURCE_DIR}/third_party/freertos/portable/MemMang/heap_4.c"
)

# Create interface library for the Heap 5 memory manager sources.
add_library(freertos-heap5 INTERFACE)

target_sources(freertos-heap5 INTERFACE
    "${CMAKE_SOURCE_DIR}/third_party/freertos/portable/MemMang/heap_5.c"
)

