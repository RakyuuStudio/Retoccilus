#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SARibbonBar" for configuration "Release"
set_property(TARGET SARibbonBar APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SARibbonBar PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libSARibbonBar.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libSARibbonBar.dll"
  )

list(APPEND _cmake_import_check_targets SARibbonBar )
list(APPEND _cmake_import_check_files_for_SARibbonBar "${_IMPORT_PREFIX}/lib/libSARibbonBar.dll.a" "${_IMPORT_PREFIX}/bin/libSARibbonBar.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
