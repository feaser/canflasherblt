# Create interface library for the board abstraction layer sources.
add_library(board INTERFACE)

target_sources(board INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/can.cpp"
)


target_include_directories(board INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}"
)

# Include board specific sources.
add_subdirectory(olimexino_stm32f3)

