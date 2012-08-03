set(SRC_CPP_FILES
)

set(INTERNAL_CPP_FILES
  org_mitk_example_gui_testapplication_Activator.cpp
  TestApplication.cpp
  TestPerspective.cpp
  TestView.cpp
  MinimalView.cpp
)

set(MOC_H_FILES
  src/internal/org_mitk_example_gui_testapplication_Activator.h

  src/internal/TestApplication.h
  src/internal/TestPerspective.h
  src/internal/TestView.h
  src/internal/MinimalView.h
)

set(UI_FILES
  src/internal/QmitkMinimalViewControls.ui
 )

set(CACHED_RESOURCE_FILES
  plugin.xml
)

set(CPP_FILES )

foreach(file ${SRC_CPP_FILES})
  set(CPP_FILES ${CPP_FILES} src/${file})
endforeach(file ${SRC_CPP_FILES})

foreach(file ${INTERNAL_CPP_FILES})
  set(CPP_FILES ${CPP_FILES} src/internal/${file})
endforeach(file ${INTERNAL_CPP_FILES})
