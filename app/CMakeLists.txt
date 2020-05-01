cmake_minimum_required(VERSION 3.12)

set(application_source)
set(application_headers)
set(application_qml)

add_subdirectory("cpp/qmlplugins/")

set(FONTS_RES "${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts.qrc")
set(ASSETS_RES "${CMAKE_CURRENT_SOURCE_DIR}/resources/assets.qrc")

set(application_resources
	${FONTS_RES}
	${ASSETS_RES}
)

set(application_subdirectories
	cpp/Core
	cpp/Mediators
	cpp/Models
	cpp/Utils
	qml
)

foreach(subdir ${application_subdirectories})
	add_subdirectory(${subdir})
endforeach()

add_library(libSource STATIC ${application_headers} ${application_source} ${application_qml})

target_include_directories(libSource PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/cpp")

if (APPLE)
	add_executable(${APP_NAME} main.cpp ${application_resources})
else()
	add_executable(${APP_NAME} WIN32 main.cpp ${application_resources})
endif()

target_link_libraries(libSource
	algorithm_utils
	app_config
	ebst
	standard_paths
	QmlPlugins
	Qt5::Core
	Qt5::Gui
	Qt5::Qml
	Qt5::Quick
)

target_link_libraries(${APP_NAME} libSource)

adjust_target_output_dir(${APP_NAME})

deduct_qtdir(QTDIR)

if(NOT QTDIR)
	message(FATAL_ERROR "QTDIR not set.")
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" OR "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	set(DEPLOY_TYPE "--release")
else()
	set(DEPLOY_TYPE "--debug")
endif()

if (APPLE)

else()
	find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS "${QTDIR}" ENV QTDIR PATH_SUFFIXES bin)

	set(windeployqt_path "${APP_ROOT_OUTPUT_DIR}")
	set(windeployqt_qmldir "${CMAKE_CURRENT_SOURCE_DIR}/qml")

	add_custom_command(
		TARGET "${APP_NAME}" POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E make_directory "${windeployqt_path}"
		COMMAND "${WINDEPLOYQT_EXECUTABLE}"
		    --qmldir "${windeployqt_qmldir}"
			--dir "${windeployqt_path}"
			${DEPLOY_TYPE}
			"$<TARGET_FILE:${APP_NAME}>"

		COMMAND "${CMAKE_COMMAND}" -E remove_directory "${windeployqt_path}/bearer/"
		COMMAND "${CMAKE_COMMAND}" -E remove_directory "${windeployqt_path}/private/"
		COMMAND "${CMAKE_COMMAND}" -E remove_directory "${windeployqt_path}/qmltooling/"

		COMMENT "Collecting Qt dependencies."
	)
endif()
