#-----------------------------------------------------------------------------
# CTK
#-----------------------------------------------------------------------------

if(MITK_USE_CTK)

  # Sanity checks
  if(DEFINED CTK_DIR AND NOT EXISTS ${CTK_DIR})
    message(FATAL_ERROR "CTK_DIR variable is defined but corresponds to non-existing directory")
  endif()

  set(proj CTK)
  set(proj_DEPENDENCIES )
  set(CTK_DEPENDS ${proj})

  if(NOT DEFINED CTK_DIR)

    set(revision_tag b721b7ca)
    #IF(${proj}_REVISION_TAG)
    #  SET(revision_tag ${${proj}_REVISION_TAG})
    #ENDIF()

    set(ctk_optional_cache_args )
    if(MITK_USE_Python)
      if(NOT MITK_USE_SYSTEM_PYTHON)
        list(APPEND proj_DEPENDENCIES Python)
      endif()
      list(APPEND ctk_optional_cache_args
           -DCTK_LIB_Scripting/Python/Widgets:BOOL=ON
           -DCTK_ENABLE_Python_Wrapping:BOOL=ON
           -DCTK_APP_ctkSimplePythonShell:BOOL=ON
           -DPYTHON_EXECUTABLE:FILEPATH=${PYTHON_EXECUTABLE}
           -DPYTHON_INCLUDE_DIR:PATH=${PYTHON_INCLUDE_DIR}
           -DPYTHON_INCLUDE_DIR2:PATH=${PYTHON_INCLUDE_DIR2}
           -DPYTHON_LIBRARY:FILEPATH=${PYTHON_LIBRARY}
      )
    else()
      list(APPEND ctk_optional_cache_args
           -DCTK_LIB_Scripting/Python/Widgets:BOOL=OFF
           -DCTK_ENABLE_Python_Wrapping:BOOL=OFF
           -DCTK_APP_ctkSimplePythonShell:BOOL=OFF
      )
    endif()

    if(MITK_USE_DCMTK)
      list(APPEND ctk_optional_cache_args
           -DDCMTK_DIR:PATH=${DCMTK_DIR}
          )
      if(NOT MITK_USE_Python)
        list(APPEND ctk_optional_cache_args
            -DDCMTK_CMAKE_DEBUG_POSTFIX:STRING=d
            )
      endif()
      list(APPEND proj_DEPENDENCIES DCMTK)
    else()
      list(APPEND ctk_optional_cache_args
           -DDCMTK_URL:STRING=${MITK_THIRDPARTY_DOWNLOAD_PREFIX_URL}/CTK_DCMTK_085525e6.tar.gz
          )
    endif()

    if(CTEST_USE_LAUNCHERS)
      list(APPEND ctk_optional_cache_args
        "-DCMAKE_PROJECT_${proj}_INCLUDE:FILEPATH=${CMAKE_ROOT}/Modules/CTestUseLaunchers.cmake"
      )
    endif()

    FOREACH(type RUNTIME ARCHIVE LIBRARY)
      IF(DEFINED CTK_PLUGIN_${type}_OUTPUT_DIRECTORY)
        LIST(APPEND mitk_optional_cache_args -DCTK_PLUGIN_${type}_OUTPUT_DIRECTORY:PATH=${CTK_PLUGIN_${type}_OUTPUT_DIRECTORY})
      ENDIF()
    ENDFOREACH()

	if(MITK_USE_Python)
	  if(NOT EXISTS "$ENV{MITK_SOURCE}")
	    message(FATAL_ERROR "Environment variable MITK_SOURCE not set, set it and build again")
	  endif()
      ExternalProject_Add(${proj}
        LIST_SEPARATOR ${sep}
        URL ${MITK_THIRDPARTY_DOWNLOAD_PREFIX_URL}/CTK_${revision_tag}.tar.gz
        #GIT_REPOSITORY https://github.com/commontk/CTK.git
        #GIT_TAG origin/master
        URL_MD5 9ebeb78c78ff9f458045e0a5ecffc73f
        PATCH_COMMAND ${PATCH_COMMAND} -N -p1 -i ${CMAKE_CURRENT_LIST_DIR}/CTK.patch
        UPDATE_COMMAND ""
        INSTALL_COMMAND ""
        CMAKE_GENERATOR ${gen}
        CMAKE_ARGS
          ${ep_common_args}
          ${ctk_optional_cache_args}
          ${ctk_qt_args}
          # The CTK PluginFramework cannot cope with
          # a non-empty CMAKE_DEBUG_POSTFIX for the plugin
          # libraries yet.
          -DCMAKE_DEBUG_POSTFIX:STRING=
          -DGit_EXECUTABLE:FILEPATH=${GIT_EXECUTABLE}
          -DGIT_EXECUTABLE:FILEPATH=${GIT_EXECUTABLE}
          -DCTK_LIB_CommandLineModules/Backend/LocalProcess:BOOL=ON
          -DCTK_LIB_CommandLineModules/Frontend/QtGui:BOOL=ON
          -DCTK_LIB_PluginFramework:BOOL=ON
          -DCTK_LIB_DICOM/Widgets:BOOL=ON
          -DCTK_LIB_XNAT/Core:BOOL=ON
          -DCTK_PLUGIN_org.commontk.eventadmin:BOOL=ON
          -DCTK_PLUGIN_org.commontk.configadmin:BOOL=ON
          -DCTK_USE_GIT_PROTOCOL:BOOL=OFF
          -DDCMTK_URL:STRING=${MITK_THIRDPARTY_DOWNLOAD_PREFIX_URL}/CTK_DCMTK_085525e6.tar.gz
          -DqRestAPI_URL:STRING=${MITK_THIRDPARTY_DOWNLOAD_PREFIX_URL}/qRestAPI_c5e4c2a7.tar.gz
          # See bug 19073
          -DPythonQt_URL:STRING=${MITK_THIRDPARTY_DOWNLOAD_PREFIX_URL}/PythonQt_36ab9c7c.tar.gz
        CMAKE_CACHE_ARGS
          ${ep_common_cache_args}
        CMAKE_CACHE_DEFAULT_ARGS
          ${ep_common_cache_default_args}
        DEPENDS ${proj_DEPENDENCIES}
       )
	   else()
		   ExternalProject_Add(${proj}
			LIST_SEPARATOR ${sep}
			URL ${MITK_THIRDPARTY_DOWNLOAD_PREFIX_URL}/CTK_${revision_tag}.tar.gz
			#GIT_REPOSITORY https://github.com/commontk/CTK.git
			#GIT_TAG origin/master
			URL_MD5 9ebeb78c78ff9f458045e0a5ecffc73f
			UPDATE_COMMAND ""
			INSTALL_COMMAND ""
			CMAKE_GENERATOR ${gen}
			CMAKE_ARGS
			  ${ep_common_args}
			  ${ctk_optional_cache_args}
			  ${ctk_qt_args}
			  # The CTK PluginFramework cannot cope with
			  # a non-empty CMAKE_DEBUG_POSTFIX for the plugin
			  # libraries yet.
			  -DCMAKE_DEBUG_POSTFIX:STRING=
			  -DGit_EXECUTABLE:FILEPATH=${GIT_EXECUTABLE}
			  -DGIT_EXECUTABLE:FILEPATH=${GIT_EXECUTABLE}
			  -DCTK_LIB_CommandLineModules/Backend/LocalProcess:BOOL=ON
			  -DCTK_LIB_CommandLineModules/Frontend/QtGui:BOOL=ON
			  -DCTK_LIB_PluginFramework:BOOL=ON
			  -DCTK_LIB_DICOM/Widgets:BOOL=ON
			  -DCTK_LIB_XNAT/Core:BOOL=ON
			  -DCTK_PLUGIN_org.commontk.eventadmin:BOOL=ON
			  -DCTK_PLUGIN_org.commontk.configadmin:BOOL=ON
			  -DCTK_USE_GIT_PROTOCOL:BOOL=OFF
			  -DDCMTK_URL:STRING=${MITK_THIRDPARTY_DOWNLOAD_PREFIX_URL}/CTK_DCMTK_085525e6.tar.gz
			  -DqRestAPI_URL:STRING=${MITK_THIRDPARTY_DOWNLOAD_PREFIX_URL}/qRestAPI_c5e4c2a7.tar.gz
			  # See bug 19073
			  -DPythonQt_URL:STRING=${MITK_THIRDPARTY_DOWNLOAD_PREFIX_URL}/PythonQt_36ab9c7c.tar.gz
			CMAKE_CACHE_ARGS
			  ${ep_common_cache_args}
			CMAKE_CACHE_DEFAULT_ARGS
			  ${ep_common_cache_default_args}
			DEPENDS ${proj_DEPENDENCIES}
		   )
	   endif()

    ExternalProject_Get_Property(${proj} binary_dir)
    set(CTK_DIR ${binary_dir})
    #set(CTK_DIR ${ep_prefix})
    #mitkFunctionInstallExternalCMakeProject(${proj})

  else()

    mitkMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")

  endif()

endif()
