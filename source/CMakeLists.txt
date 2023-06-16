# Create interface library for the application sources.
add_library(application INTERFACE)

target_sources(application INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/application.cpp"
)

target_include_directories(application INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}"
)

target_link_libraries(application INTERFACE
  microtbx
  microtbx-extra-cpp
  board
)

# Include board specific sources.
add_subdirectory(board)

