add_library(algorithm_utils INTERFACE)

target_sources(algorithm_utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/algorithmUtils.h)

target_include_directories(algorithm_utils INTERFACE "${PROJECT_SOURCE_DIR}/algorithm_utils")
