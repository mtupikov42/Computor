cmake_minimum_required(VERSION 3.12)

set(APP_NAME "AppName" CACHE STRING "Application Name")
set(APP_COMPANY_NAME "AppCompanyName" CACHE STRING "Application Company Name")
set(APP_ROOT_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/out")

project(${APP_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 QUIET REQUIRED COMPONENTS
	Core
	Gui
	Qml
	Quick
	QuickControls2
)

if(WIN32)
	find_package(Qt5WinExtras REQUIRED QUIET)
endif()

include("AddDirectory")
include("Utils")

set(APP_VERSION_STRING "0.1.0" CACHE STRING "Application Version String")
parse_version_string("${APP_VERSION_STRING}" APP_VERSION_MAJOR APP_VERSION_MINOR APP_VERSION_PATCH)

add_subdirectory(algorithm_utils)
add_subdirectory(app_config)
add_subdirectory(standard_paths)
add_subdirectory(app)
