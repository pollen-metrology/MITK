#-----------------------------------------------------------------------------
# OpenCV
#-----------------------------------------------------------------------------

if(MITK_USE_OpenCV)

  # Sanity checks
  if(DEFINED OpenCV_DIR AND NOT EXISTS ${OpenCV_DIR})
    message(FATAL_ERROR "OpenCV_DIR variable is defined but corresponds to non-existing directory")
  endif()

  set(proj OpenCV)
  set(proj_DEPENDENCIES)
  set(OpenCV_DEPENDS ${proj})

  if(NOT DEFINED OpenCV_DIR)

    set(additional_cmake_args
      -DBUILD_opencv_java:BOOL=OFF
      -DBUILD_opencv_ts:BOOL=OFF
      -DBUILD_PERF_TESTS:BOOL=OFF
    )

    if(MITK_USE_Python)
      if(NOT MITK_USE_SYSTEM_PYTHON)
        list(APPEND proj_DEPENDENCIES Python Numpy)
        # export python home
        set(ENV{PYTHONHOME} "${Python_DIR}")
        set(CV_PACKAGE_PATH -DPYTHON_PACKAGES_PATH:PATH=${MITK_PYTHON_SITE_DIR})
      else()
        set(CV_PACKAGE_PATH -DPYTHON_PACKAGES_PATH:PATH=${ep_prefix}/lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages)
      endif()

      list(APPEND additional_cmake_args
         -DBUILD_opencv_python:BOOL=ON
         -DBUILD_NEW_PYTHON_SUPPORT:BOOL=ON
         -DPYTHON_DEBUG_LIBRARY:FILEPATH=${PYTHON_DEBUG_LIBRARY}
         -DPYTHON_EXECUTABLE:FILEPATH=${PYTHON_EXECUTABLE}
         -DPYTHON_INCLUDE_DIR:PATH=${PYTHON_INCLUDE_DIR}
         -DPYTHON_INCLUDE_DIR2:PATH=${PYTHON_INCLUDE_DIR2}
         -DPYTHON_LIBRARY:FILEPATH=${PYTHON_LIBRARY}
         ${CV_PACKAGE_PATH}
         #-DPYTHON_LIBRARIES=${PYTHON_LIBRARY}
         #-DPYTHON_DEBUG_LIBRARIES=${PYTHON_DEBUG_LIBRARIES}
          )
    else()
      list(APPEND additional_cmake_args
         -DBUILD_opencv_python:BOOL=OFF
         -DBUILD_NEW_PYTHON_SUPPORT:BOOL=OFF
          )
    endif()

    # 12-05-02, muellerm, added QT usage by OpenCV if QT is used in MITK
    # 12-09-11, muellerm, removed automatic usage again, since this will struggle with the MITK Qt application object
    if(MITK_USE_QT)
      list(APPEND additional_cmake_args
           -DWITH_QT:BOOL=OFF
           -DWITH_QT_OPENGL:BOOL=OFF
           -DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_QMAKE_EXECUTABLE}
          )
    endif()

    if(CTEST_USE_LAUNCHERS)
      list(APPEND additional_cmake_args
        "-DCMAKE_PROJECT_${proj}_INCLUDE:FILEPATH=${CMAKE_ROOT}/Modules/CTestUseLaunchers.cmake"
      )
    endif()

    set(openCVURL ${OPENCV_LIBRARY_ARCHIVE})
    set(opencv_url ${openCVURL})
    #set(opencv_url_md5 54fe3dba49ea276ec0228f8819e653bc)

    ExternalProject_Add(${proj}
      LIST_SEPARATOR ${sep}
      URL ${opencv_url}
      URL_MD5 ${opencv_url_md5}
      # Related bug: http://bugs.mitk.org/show_bug.cgi?id=5912
      # PATCH_COMMAND ${PATCH_COMMAND} -N -p1 -i ${CMAKE_CURRENT_LIST_DIR}/OpenCV-2.4.11.patch
      CMAKE_GENERATOR ${gen}
      CMAKE_ARGS
        ${ep_common_args}
        -DBUILD_JPEG=ON
        -DBUILD_TIFF=ON
        -DBUILD_PNG=ON
        -DBUILD_TESTS:BOOL=OFF
        -DBUILD_DOCS:BOOL=OFF
        -DBUILD_EXAMPLES:BOOL=OFF
        -DBUILD_DOXYGEN_DOCS:BOOL=OFF
        -DWITH_CUDA:BOOL=OFF
        -DWITH_IPP:BOOL=${WITH_IPP}
        -DEIGEN_INCLUDE_PATH:PATH=${EIGEN_INCLUDE_PATH}
        -DWITH_VFW:BOOL=${WITH_VFW}
        -DWITH_V4L:BOOL=FALSE
        -DWITH_GSTREAMER:BOOL=FALSE 
        -DWITH_VTK:BOOL=${WITH_VTK}
        -DOPENCV_EXTRA_MODULES_PATH:PATH=${OPENCV_EXTRA_MODULES_PATH}
        -DBUILD_opencv_adas:BOOL=OFF 
        -DBUILD_opencv_bgsegm:BOOL=OFF 
        -DBUILD_opencv_bioinspired:BOOL=OFF 
        -DBUILD_opencv_ccalib:BOOL=OFF 
        -DBUILD_opencv_datasettools:BOOL=OFF 
        -DBUILD_opencv_latentsvm:BOOL=OFF 
        -DBUILD_opencv_line_descriptor:BOOL=OFF
        -DBUILD_opencv_matlab:BOOL=OFF 
        -DBUILD_opencv_optflow:BOOL=OFF 
        -DBUILD_opencv_reg:BOOL=OFF
        -DBUILD_opencv_rgbd:BOOL=OFF 
        -DBUILD_opencv_saliency:BOOL=OFF 
        -DBUILD_opencv_surface_matching:BOOL=OFF
        -DBUILD_opencv_text:BOOL=OFF
        -DBUILD_opencv_tracking:BOOL=OFF
        -DBUILD_opencv_xfeatures2d:BOOL=OFF
        -DBUILD_opencv_xobjdetect:BOOL=OFF
        -DBUILD_opencv_stereo:BOOL=OFF
        -DBUILD_opencv_cvv:BOOL=OFF
        -DBUILD_opencv_hdf:BOOL=OFF
        ${additional_cmake_args}
      CMAKE_CACHE_ARGS
        ${ep_common_cache_args}
      CMAKE_CACHE_DEFAULT_ARGS
        ${ep_common_cache_default_args}
      DEPENDS ${proj_DEPENDENCIES}
    )

    set(OpenCV_DIR ${ep_prefix})
    mitkFunctionInstallExternalCMakeProject(${proj})

  else()

    mitkMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")

  endif()

endif()
