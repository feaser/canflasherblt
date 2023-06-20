# Include FreeRTOS specific third party components.
include("${CMAKE_SOURCE_DIR}/.cmake/FreeRTOS.cmake")

# Create interface library for the application sources.
add_library(application INTERFACE)

target_sources(application INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/application.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/controlloop.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/indicator.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/gateway.cpp"
)

target_include_directories(application INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}"
)

target_link_libraries(application INTERFACE
  freertos
  freertos-addon-cpp  
  microtbx
  microtbx-extra-cpp
  microtbx-extra-freertos
  board
)

# Include board specific sources.
add_subdirectory(board)

