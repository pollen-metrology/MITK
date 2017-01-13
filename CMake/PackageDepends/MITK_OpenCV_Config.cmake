macro(REMOVE_LAST_PATH_ELEMENT_IF_EQUALS INPUT_VAR TARGET_VALUE)
  STRING( REPLACE "/" ";" INPUT_VARLIST ${${INPUT_VAR}} )
  LIST( LENGTH INPUT_VARLIST INPUT_VARLIST_LENGTH )

  SET(INPUT_VARLIST_LAST_INDEX ${INPUT_VARLIST_LENGTH})
  MATH( EXPR INPUT_VARLIST_LAST_INDEX "${INPUT_VARLIST_LENGTH}-1" )
  LIST( GET INPUT_VARLIST ${INPUT_VARLIST_LAST_INDEX} INPUT_VARLAST_ELEMENT )

  if(INPUT_VARLAST_ELEMENT MATCHES "^${TARGET_VALUE}$")
    LIST( REMOVE_AT INPUT_VARLIST ${INPUT_VARLIST_LAST_INDEX} )
  endif()

  set(NEW_VAR "")
  foreach(folderPart ${INPUT_VARLIST})
    if(NOT NEW_VAR)
      if(NOT WIN32)
        set(folderPart "/${folderPart}")
      endif()
      set(NEW_VAR "${folderPart}")
    else()
      set(NEW_VAR "${NEW_VAR}/${folderPart}")
    endif()
  endforeach()

  set(${INPUT_VAR} ${NEW_VAR})
endmacro()

list(APPEND ALL_LIBRARIES ${OpenCV_LIBS})
list(APPEND ALL_INCLUDE_DIRECTORIES ${OpenCV_INCLUDE_DIRS})

# adding option for videoinput library on windows (for directshow based frame grabbing)
if(WIN32)
  option(MITK_USE_videoInput "Use videoInput (DirectShow wrapper) library" OFF)
endif(WIN32)
