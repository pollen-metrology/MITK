MITK_CREATE_MODULE(
  INCLUDE_DIRS Algorithms DataManagement IO Rendering
  DEPENDS MitkCore MitkSceneSerializationBase MitkLegacyGL MitkOverlays
  PACKAGE_DEPENDS ITK|ITKReview
  # AUTOLOAD_WITH MitkCore TODO: Create IO Submodule and autoload that one instead.
  WARNINGS_AS_ERRORS
)

if(CMAKE_COMPILER_IS_GNUXX OR CMAKE_COMPILER_IS_GNUCC)
        mitkFunctionCheckCAndCXXCompilerFlags("-Wno-error=deprecated" CMAKE_C_FLAGS CMAKE_CXX_FLAGS)
        mitkFunctionCheckCAndCXXCompilerFlags("-Wno-error=deprecated-declarations" CMAKE_C_FLAGS CMAKE_CXX_FLAGS)
endif(CMAKE_COMPILER_IS_GNUXX OR CMAKE_COMPILER_IS_GNUCC)

add_subdirectory(Testing)
