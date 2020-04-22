configure_file("app_config_macros.h.in" "${CMAKE_CURRENT_BINARY_DIR}/app_config_macros.h")

add_library(app_config STATIC
	app_config.h
	app_config.cpp
	app_config_macros.h.in
	"${CMAKE_CURRENT_BINARY_DIR}/app_config_macros.h"
)

target_include_directories(app_config INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(app_config PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
