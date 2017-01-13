MITK_CREATE_MODULE(
  DEPENDS MitkImageStatistics MitkQtWidgets
  PACKAGE_DEPENDS
    PUBLIC Qwt CTK|CTKWidgets Qt5|WebKitWidgets
    PRIVATE Qt5|Concurrent+Svg+Xml
  WARNINGS_AS_ERRORS
)


if(CMAKE_COMPILER_IS_GNUXX OR CMAKE_COMPILER_IS_GNUCC)
        mitkFunctionCheckCAndCXXCompilerFlags("-Wno-error=deprecated" CMAKE_C_FLAGS CMAKE_CXX_FLAGS)
        mitkFunctionCheckCAndCXXCompilerFlags("-Wno-error=deprecated-declarations" CMAKE_C_FLAGS CMAKE_CXX_FLAGS)
endif(CMAKE_COMPILER_IS_GNUXX OR CMAKE_COMPILER_IS_GNUCC)

