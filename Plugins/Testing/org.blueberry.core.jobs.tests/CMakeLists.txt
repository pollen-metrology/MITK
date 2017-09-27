
include(mitkTestPluginGenerator)

project(org_blueberry_core_jobs_tests)

mitk_create_plugin(
  EXPORT_DIRECTIVE BERRY_RUNTIME_TESTS
  PACKAGE_DEPENDS PRIVATE CppUnit
  TEST_PLUGIN
)

include(mitkFunctionTestPlugin)
set(BLUEBERRY_ENABLE_GUI_TESTING true)
MACRO_TEST_UIPLUGIN()