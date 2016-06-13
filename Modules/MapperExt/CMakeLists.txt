MITK_CREATE_MODULE(DEPENDS MitkDataTypesExt MitkLegacyGL
                   PACKAGE_DEPENDS VTK|vtkRenderingVolumeOpenGL
                   
                  )

add_subdirectory(test)

