set(headers
	AppQmlPlugin.h
	AssetsProvider.h
	ControlsColorPalette.h
	CustomWindow.h
	CustomWindowImpl.h
	GenericColorPalette.h
	QmlColorPalette.h
	QmlListModel.h
	SizeProvider.h
	SpecialSymbols.h
	Theme.h
)

set(source
	AppQmlPlugin.cpp
	AssetsProvider.cpp
	ControlsColorPalette.cpp
	CustomWindow.cpp
	CustomWindowImpl.cpp
	GenericColorPalette.cpp
	QmlColorPalette.cpp
	QmlListModel.cpp
	SizeProvider.cpp
	SpecialSymbols.cpp
	Theme.cpp
)

if(WIN32)
	list(APPEND headers
		WinCustomWindowImpl.h
	)

	list(APPEND source
		WinCustomWindowImpl.cpp
	)
endif()

if(APPLE)
	list(APPEND headers
		MacCustomWindowImpl.h
	)

	list(APPEND source
		MacCustomWindowImpl.cpp
	)
endif()

add_library(QmlPlugins STATIC ${headers} ${source})

target_link_libraries(QmlPlugins
	Qt5::Core
	Qt5::Qml
	Qt5::Quick
	algorithm_utils
)

if(WIN32)
	target_link_libraries(QmlPlugins Dwmapi.lib Qt5::WinExtras)
endif()

target_include_directories(QmlPlugins INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(QmlPlugins PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
