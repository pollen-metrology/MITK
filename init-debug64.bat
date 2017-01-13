call C:\dev\pollen-vars.bat msvc14 amd64
call C:\dev\compilers\msvc14\VC\vcvarsall.bat amd64

set QTDIR=C:/dev/dist/msvc14/amd64/qt551

mkdir debug
cd debug

cmake .. ^
-G"Visual Studio 14 2015 Win64" ^
-DCMAKE_GENERATOR="Visual Studio 14 2015 Win64" ^
-DCMAKE_INSTALL_PREFIX=c:\dev\dist\msvc14\amd64\mitk\debug ^
-DDESIRED_QT_VERSION=5 ^
-DMITK_USE_QT=ON ^
-DMITK_USE_BOOST=ON ^
-DQt5_DIR=C:\dev\dist\msvc14\amd64\qt551\lib\cmake\Qt5 ^
-DCMAKE_BUILD_TYPE=Debug ^
-DBUILD_SHARED_LIBS=true ^
-DVTK_USE_QVTK=ON ^
-DMITK_USE_DCMTK:BOOL=OFF ^
-DVTK_USE_QVTK_QTOPENGL=ON ^
-DEigen_DIR=C:/dev/dist/msvc14/amd64/eigen/release ^
-DEigen_INCLUDE_DIR=C:/dev/dist/msvc14/amd64/eigen/release/include/eigen3 ^
-DMITK_USE_Redland=ON ^
-DMITK_USE_OpenCV=ON ^
-DMITK_BUILD_ALL_PLUGINS:BOOL=ON ^
-DBLUEBERRY_DEBUG_SMARTPOINTER:BOOL=true ^
-DWITH_IPP:BOOL=FALSE ^
-DEIGEN_INCLUDE_PATH:PATH=C:/dev/dist/msvc14/amd64/eigen/release/include/eigen3 ^
-DWITH_VTK:BOOL=NO ^
-DOPENCV_LIBRARY_ARCHIVE=c:/dev/libraries/archives/opencv-3.1.0.zip ^
-DOPENCV_EXTRA_MODULES_PATH=c:/dev/libraries/unpack/opencv_contrib-master/modules ^
-DBUILD_TESTING:BOOL=YES
