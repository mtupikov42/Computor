@echo off

for %%a in (%*) do (
    call set "%%~1=%%~2"
    shift
)

rem

:: msvc

call :invoke_msvc_vcvarsall
if not "%errorlevel%"=="0" exit /b 10

:: qt

call :invoke_qt_qtenv2
if not "%errorlevel%"=="0" exit /b 11

:: cmake

set cmake_found=0

call :test_binary cmake
if "%errorlevel%"=="0" (
	echo Found cmake in the default environment
	set cmake_found=1
)

call :fix_cmake_env
call :test_binary cmake
if "%errorlevel%"=="0" (
	echo Found cmake in CMAKEPATH
	set cmake_found=1
)

call :fix_cmake_paths
call :test_binary cmake
if "%errorlevel%"=="0" (
	echo Found cmake in ProgramFiles
	set cmake_found=1
)

if not "%cmake_found%"=="1" (
	echo CMake not found
	exit /b 69
)

:: build

call :start_build

exit /b %errorlevel%

rem

:: funcs

:start_build
if "%BUILD_CONFIGURATIONS%"=="Release"  (
	echo Will build only Release
	call :build_cfg RelWithDebInfo
) else (
	echo Will build only Debug
	call :build_cfg debug
)
exit /b %errorlevel%

:build_cfg
echo Building configuration '%1'
set buildtype=%1
set builddir="../build-app-%buildtype%"
if not exist %builddir% mkdir %builddir%
if not "%EXTERNAL_PROJECT_VERSION%"=="" (
	set ext_version="-DAPP_VERSION_STRING=%EXTERNAL_PROJECT_VERSION%"
) else (
	set ext_version =
)
call cmake . -B%builddir% -DCMAKE_PREFIX_PATH="%QTDIR%\lib\cmake" -G"Visual Studio 15 2017" -A x64 -DCMAKE_BUILD_TYPE=%buildtype% %ext_version%
echo lol4
call cmake --build %builddir% --config %buildtype%
exit /b %errorlevel%

:test_binary
call where %1 2>nul 1>nul
echo Searching for '%1', result='%errorlevel%'
exit /b %errorlevel%

:fix_cmake_env
set PATH=%PATH%;%CMAKEPATH%
exit /b

:fix_cmake_paths
set PATH=%PATH%;%ProgramFiles(x86)%\CMake\bin;%ProgramFiles%\CMake\bin
exit /b

:invoke_msvc_vcvarsall
set msvc2017Pre1=%ProgramFiles(x86)%\Microsoft Visual Studio\2017
set msvc2017Pre2=%ProgramFiles%\Microsoft Visual Studio\2017
set msvc2017commonPost=VC\Auxiliary\Build\vcvars32.bat
set msvc2017paths="%msvc2017Pre1%\Community\%msvc2017commonPost%" "%msvc2017Pre2%\Community\%msvc2017commonPost%" "%msvc2017Pre1%\Professional\%msvc2017commonPost%" "%msvc2017Pre2%\Professional\%msvc2017commonPost%" "%msvc2017Pre1%\Enterprise\%msvc2017commonPost%" "%msvc2017Pre2%\Enterprise\%msvc2017commonPost%"
if "%VISUAL_STUDIO_VERSION%"=="msvc2013" (
	call "%VS120COMNTOOLS%\..\..\VC\vcvarsall.bat"
) else if "%VISUAL_STUDIO_VERSION%"=="msvc2015" (
	call "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" amd64_x86
) else if "%VISUAL_STUDIO_VERSION%"=="msvc2017" (
	for %%P in (%msvc2017paths%) do (
		if exist %%P (
			call %%P amd64_x86
			exit /b !errorlevel!
		)
	)
)
exit /b %errorlevel%

:invoke_qt_qtenv2
call :test_binary qmake
if "%errorlevel%"=="0" exit /b 0
pushd %CD%
call %QTDIR%\bin\qtenv2.bat
popd
exit /b %errorlevel%
