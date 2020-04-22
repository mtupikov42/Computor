add_library(standard_paths STATIC
	StandardPaths.h
	StandardPaths.cpp
)

target_link_libraries(standard_paths Qt5::Core)

target_include_directories(standard_paths INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(standard_paths PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
