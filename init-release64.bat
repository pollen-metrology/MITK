call c:\dev\pollen-vars.bat amd64 release
call C:\dev\compilers\msvc14\VC\vcvarsall.bat amd64

set INSTALL_PREFIX=c:\dev\dist\msvc14\amd64\mitk\release
set MITK_SOURCE=%CD%
set QTDIR=C:/dev/dist/msvc14/amd64/qt551

mkdir release
cd release

cmake .. ^
-G"Visual Studio 14 2015 Win64" ^
-DCMAKE_GENERATOR="Visual Studio 14 2015 Win64" ^
-DCMAKE_INSTALL_PREFIX=%INSTALL_PREFIX% ^
-DDESIRED_QT_VERSION=5 ^
-DMITK_USE_QT=ON ^
-DMITK_USE_BOOST=ON ^
-DQt5_DIR=C:/dev/dist/msvc14/amd64/qt551/lib/cmake/Qt5 ^
-DCMAKE_BUILD_TYPE=Release ^
-DBUILD_SHARED_LIBS=true ^
-DVTK_USE_QVTK=ON ^
-DVTK_USE_QVTK_QTOPENGL=ON ^
-DEigen_DIR=C:/dev/dist/msvc14/amd64/eigen/release ^
-DEigen_INCLUDE_DIR=C:/dev/dist/msvc14/amd64/eigen/release/include/eigen3 ^
-DMITK_USE_Redland=OFF ^
-DMITK_USE_SimpleITK=OFF ^
-DMITK_USE_PYTHON:BOOL=OFF ^
-DMITK_BUILD_ALL_PLUGINS:BOOL=ON ^
-DMITK_SHOW_CONSOLE_WINDOW=OFF ^
-DMITK_USE_OpenCV=ON ^
-DWITH_IPP:BOOL=FALSE ^
-DWITH_VFW:BOOL=FALSE ^
-DWITH_DIRECTX:BOOL=FALSE ^
-DMITK_USE_Eigen=YES ^
-DEigen_DIR:PATH=C:/dev/dist/msvc14/amd64/eigen/release/include/eigen3 ^
-DEigen_INCLUDE_DIR=C:/dev/dist/msvc14/amd64/eigen/release/include/eigen3 ^
-DEIGEN_INCLUDE_PATH:PATH=C:/dev/dist/msvc14/amd64/eigen/release/include/eigen3 ^
-DWITH_VTK:BOOL=NO ^
-DBUILD_TESTING:BOOL=YES ^
-DOPENCV_LIBRARY_ARCHIVE=c:/dev/libraries/archives/opencv-3.4.4.zip ^
-DOPENCV_EXTRA_MODULES_PATH=c:/dev/libraries/unpack/opencv_contrib-3.4.4/modules

msbuild MITK-superbuild.sln /m:4 /property:Configuration=Release;platform="x64" /t:ALL_BUILD
