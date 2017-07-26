#!/bin/bash

mkdir release
cd release

cmake .. \
-G"Eclipse CDT4 - Unix Makefiles" \
-DDESIRED_QT_VERSION=5 \
-DMITK_USE_QT=ON \
-DMITK_USE_BOOST=ON \
-DCMAKE_BUILD_TYPE=Release \
-DBUILD_SHARED_LIBS=true \
-DVTK_USE_QVTK=ON \
-DVTK_USE_QVTK_QTOPENGL=ON \
-DMITK_USE_Redland=ON \
-DMITK_BUILD_ALL_PLUGINS:BOOL=ON \
-DMITK_SHOW_CONSOLE_WINDOW=OFF \
-DMITK_USE_OpenCV=ON \
-DMITK_USE_Python=ON \
-DMITK_USE_SYSTEM_PYTHON=ON \
-DPYTHON_EXECUTABLE=/usr/bin/python2.7 \
-DPYTHON_INCLUDE_DIR=/usr/include/python2.7/ \
-DPYTHON_LIBRARY=/usr/lib/x86_64-linux-gnu/libpython2.7.so.1.0 \
-DPYTHON_PACKAGES_PATH=/usr/lib/python2.7/dist-packages/ \
-DITK_WRAP_PYTHON:BOOL=true \
-DWITH_IPP:BOOL=FALSE \
-DWITH_VFW:BOOL=FALSE \
-DWITH_VTK:BOOL=NO \
-DOPENCV_LIBRARY_ARCHIVE=/pollen/libraries/opencv-3.1.0.zip \
-DOPENCV_EXTRA_MODULES_PATH:PATH=/pollen/libraries/opencv_contrib-3.1.0/modules \
-DBUILD_TESTING:BOOL=YES \
-DEIGEN_INCLUDE_PATH:PATH=/usr/include/eigen3 \
-DEigen_INCLUDE_DIR=/usr/include/eigen3 \
-DCMAKE_INSTALL_PREFIX=/pollen/dist/gcc/amd64/mitk-release64
