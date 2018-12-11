#!/bin/bash

mkdir debug
cd debug
cmake .. \
-G"Eclipse CDT4 - Unix Makefiles" \
-DDESIRED_QT_VERSION=5 \
-DMITK_USE_QT=ON \
-DMITK_USE_BOOST=ON \
-DCMAKE_BUILD_TYPE=Debug \
-DBUILD_SHARED_LIBS=true \
-DVTK_USE_QVTK=ON \
-DMITK_USE_PYTHON:BOOL=OFF \
-DVTK_USE_QVTK_QTOPENGL=ON \
-DMITK_USE_Redland=OFF \
-DMITK_BUILD_ALL_PLUGINS:BOOL=ON \
-DMITK_SHOW_CONSOLE_WINDOW=OFF \
-DMITK_USE_OpenCV=ON \
-DMITK_USE_Python=OFF \
-DWITH_IPP:BOOL=FALSE \
-DWITH_DIRECTX:BOOL=FALSE \
-DWITH_VFW:BOOL=FALSE \
-DWITH_VTK:BOOL=NO \
-DBUILD_TESTING:BOOL=YES \
-DOPENCV_LIBRARY_ARCHIVE=/pollen/libraries/opencv-3.4.4.zip \
-DOPENCV_EXTRA_MODULES_PATH:PATH=/pollen/libraries/opencv_contrib-3.4.4/modules \
-DEIGEN_INCLUDE_PATH:PATH=/pollen/dist/gcc/amd64/eigen/release/include/eigen3 \
-DEigen_INCLUDE_DIR=/pollen/dist/gcc/amd64/eigen/release/include/eigen3 \
-DCMAKE_INSTALL_PREFIX=/pollen/dist/gcc/amd64/mitk-debug64
