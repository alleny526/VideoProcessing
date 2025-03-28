# Generated by CMake

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.8)
   message(FATAL_ERROR "CMake >= 2.8.0 required")
endif()
if(CMAKE_VERSION VERSION_LESS "2.8.12")
   message(FATAL_ERROR "CMake >= 2.8.12 required")
endif()
cmake_policy(PUSH)
cmake_policy(VERSION 2.8.12...3.27)
#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Protect against multiple inclusion, which would fail when already imported targets are added once more.
set(_cmake_targets_defined "")
set(_cmake_targets_not_defined "")
set(_cmake_expected_targets "")
foreach(_cmake_expected_target IN ITEMS wx::wxbase wx::wxnet wx::wxcore wx::wxadv wx::wxaui wx::wxhtml wx::wxpropgrid wx::wxribbon wx::wxrichtext wx::wxwebview wx::wxstc wx::wxxrc wx::wxmedia wx::wxgl wx::wxqa wx::wxxml)
  list(APPEND _cmake_expected_targets "${_cmake_expected_target}")
  if(TARGET "${_cmake_expected_target}")
    list(APPEND _cmake_targets_defined "${_cmake_expected_target}")
  else()
    list(APPEND _cmake_targets_not_defined "${_cmake_expected_target}")
  endif()
endforeach()
unset(_cmake_expected_target)
if(_cmake_targets_defined STREQUAL _cmake_expected_targets)
  unset(_cmake_targets_defined)
  unset(_cmake_targets_not_defined)
  unset(_cmake_expected_targets)
  unset(CMAKE_IMPORT_FILE_VERSION)
  cmake_policy(POP)
  return()
endif()
if(NOT _cmake_targets_defined STREQUAL "")
  string(REPLACE ";" ", " _cmake_targets_defined_text "${_cmake_targets_defined}")
  string(REPLACE ";" ", " _cmake_targets_not_defined_text "${_cmake_targets_not_defined}")
  message(FATAL_ERROR "Some (but not all) targets in this export set were already defined.\nTargets Defined: ${_cmake_targets_defined_text}\nTargets not yet defined: ${_cmake_targets_not_defined_text}\n")
endif()
unset(_cmake_targets_defined)
unset(_cmake_targets_not_defined)
unset(_cmake_expected_targets)


# Compute the installation prefix relative to this file.
get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
if(_IMPORT_PREFIX STREQUAL "/")
  set(_IMPORT_PREFIX "")
endif()

# Create imported target wx::wxbase
add_library(wx::wxbase SHARED IMPORTED)

set_target_properties(wx::wxbase PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "__WXOSX_COCOA__;__WXMAC__;__WXOSX__;WXUSINGDLL;_FILE_OFFSET_BITS=64"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/lib/wx/include/osx_cocoa-unicode-3.3;${_IMPORT_PREFIX}/include/wx-3.3"
  INTERFACE_LINK_LIBRARIES "-framework CoreFoundation;-framework Carbon;-framework Cocoa;-framework IOKit;-framework QuartzCore"
)

# Create imported target wx::wxnet
add_library(wx::wxnet SHARED IMPORTED)

set_target_properties(wx::wxnet PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "__WXOSX_COCOA__;__WXMAC__;__WXOSX__;WXUSINGDLL;_FILE_OFFSET_BITS=64"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/lib/wx/include/osx_cocoa-unicode-3.3;${_IMPORT_PREFIX}/include/wx-3.3"
  INTERFACE_LINK_LIBRARIES "wx::wxbase"
)

# Create imported target wx::wxcore
add_library(wx::wxcore SHARED IMPORTED)

set_target_properties(wx::wxcore PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "__WXOSX_COCOA__;__WXMAC__;__WXOSX__;WXUSINGDLL;_FILE_OFFSET_BITS=64"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/lib/wx/include/osx_cocoa-unicode-3.3;${_IMPORT_PREFIX}/include/wx-3.3"
  INTERFACE_LINK_LIBRARIES "wx::wxbase;-framework AudioToolbox;-framework WebKit"
)

# Create imported target wx::wxadv
add_library(wx::wxadv SHARED IMPORTED)

set_target_properties(wx::wxadv PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "__WXOSX_COCOA__;__WXMAC__;__WXOSX__;WXUSINGDLL;_FILE_OFFSET_BITS=64"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/lib/wx/include/osx_cocoa-unicode-3.3;${_IMPORT_PREFIX}/include/wx-3.3"
  INTERFACE_LINK_LIBRARIES "wx::wxbase;wx::wxcore"
)

# Create imported target wx::wxaui
add_library(wx::wxaui SHARED IMPORTED)

set_target_properties(wx::wxaui PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "__WXOSX_COCOA__;__WXMAC__;__WXOSX__;WXUSINGDLL;_FILE_OFFSET_BITS=64"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/lib/wx/include/osx_cocoa-unicode-3.3;${_IMPORT_PREFIX}/include/wx-3.3"
  INTERFACE_LINK_LIBRARIES "wx::wxbase;wx::wxcore"
)

# Create imported target wx::wxhtml
add_library(wx::wxhtml SHARED IMPORTED)

set_target_properties(wx::wxhtml PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "__WXOSX_COCOA__;__WXMAC__;__WXOSX__;WXUSINGDLL;_FILE_OFFSET_BITS=64"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/lib/wx/include/osx_cocoa-unicode-3.3;${_IMPORT_PREFIX}/include/wx-3.3"
  INTERFACE_LINK_LIBRARIES "wx::wxbase;wx::wxcore"
)

# Create imported target wx::wxpropgrid
add_library(wx::wxpropgrid SHARED IMPORTED)

set_target_properties(wx::wxpropgrid PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "__WXOSX_COCOA__;__WXMAC__;__WXOSX__;WXUSINGDLL;_FILE_OFFSET_BITS=64"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/lib/wx/include/osx_cocoa-unicode-3.3;${_IMPORT_PREFIX}/include/wx-3.3"
  INTERFACE_LINK_LIBRARIES "wx::wxbase;wx::wxcore"
)

# Create imported target wx::wxribbon
add_library(wx::wxribbon SHARED IMPORTED)

set_target_properties(wx::wxribbon PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "__WXOSX_COCOA__;__WXMAC__;__WXOSX__;WXUSINGDLL;_FILE_OFFSET_BITS=64"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/lib/wx/include/osx_cocoa-unicode-3.3;${_IMPORT_PREFIX}/include/wx-3.3"
  INTERFACE_LINK_LIBRARIES "wx::wxbase;wx::wxcore"
)

# Create imported target wx::wxrichtext
add_library(wx::wxrichtext SHARED IMPORTED)

set_target_properties(wx::wxrichtext PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "__WXOSX_COCOA__;__WXMAC__;__WXOSX__;WXUSINGDLL;_FILE_OFFSET_BITS=64"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/lib/wx/include/osx_cocoa-unicode-3.3;${_IMPORT_PREFIX}/include/wx-3.3"
  INTERFACE_LINK_LIBRARIES "wx::wxbase;wx::wxcore"
)

# Create imported target wx::wxwebview
add_library(wx::wxwebview SHARED IMPORTED)

set_target_properties(wx::wxwebview PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "__WXOSX_COCOA__;__WXMAC__;__WXOSX__;WXUSINGDLL;_FILE_OFFSET_BITS=64"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/lib/wx/include/osx_cocoa-unicode-3.3;${_IMPORT_PREFIX}/include/wx-3.3"
  INTERFACE_LINK_LIBRARIES "wx::wxbase;wx::wxcore;-framework WebKit"
)

# Create imported target wx::wxstc
add_library(wx::wxstc SHARED IMPORTED)

set_target_properties(wx::wxstc PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "__WXOSX_COCOA__;__WXMAC__;__WXOSX__;WXUSINGDLL;_FILE_OFFSET_BITS=64"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/lib/wx/include/osx_cocoa-unicode-3.3;${_IMPORT_PREFIX}/include/wx-3.3"
  INTERFACE_LINK_LIBRARIES "wx::wxbase;wx::wxcore"
)

# Create imported target wx::wxxrc
add_library(wx::wxxrc SHARED IMPORTED)

set_target_properties(wx::wxxrc PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "__WXOSX_COCOA__;__WXMAC__;__WXOSX__;WXUSINGDLL;_FILE_OFFSET_BITS=64"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/lib/wx/include/osx_cocoa-unicode-3.3;${_IMPORT_PREFIX}/include/wx-3.3"
  INTERFACE_LINK_LIBRARIES "wx::wxbase;wx::wxcore"
)

# Create imported target wx::wxmedia
add_library(wx::wxmedia SHARED IMPORTED)

set_target_properties(wx::wxmedia PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "__WXOSX_COCOA__;__WXMAC__;__WXOSX__;WXUSINGDLL;_FILE_OFFSET_BITS=64"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/lib/wx/include/osx_cocoa-unicode-3.3;${_IMPORT_PREFIX}/include/wx-3.3"
  INTERFACE_LINK_LIBRARIES "wx::wxbase;wx::wxcore;-framework AVFoundation;-framework CoreMedia;-weak_framework AVKit"
)

# Create imported target wx::wxgl
add_library(wx::wxgl SHARED IMPORTED)

set_target_properties(wx::wxgl PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "__WXOSX_COCOA__;__WXMAC__;__WXOSX__;WXUSINGDLL;_FILE_OFFSET_BITS=64"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/lib/wx/include/osx_cocoa-unicode-3.3;${_IMPORT_PREFIX}/include/wx-3.3"
  INTERFACE_LINK_LIBRARIES "wx::wxbase;wx::wxcore;OpenGL::GLU;OpenGL::GL;/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk/System/Library/Frameworks/OpenGL.framework;/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk/System/Library/Frameworks/OpenGL.framework"
)

# Create imported target wx::wxqa
add_library(wx::wxqa SHARED IMPORTED)

set_target_properties(wx::wxqa PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "__WXOSX_COCOA__;__WXMAC__;__WXOSX__;WXUSINGDLL;_FILE_OFFSET_BITS=64"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/lib/wx/include/osx_cocoa-unicode-3.3;${_IMPORT_PREFIX}/include/wx-3.3"
  INTERFACE_LINK_LIBRARIES "wx::wxbase;wx::wxcore;wx::wxxml"
)

# Create imported target wx::wxxml
add_library(wx::wxxml SHARED IMPORTED)

set_target_properties(wx::wxxml PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "__WXOSX_COCOA__;__WXMAC__;__WXOSX__;WXUSINGDLL;_FILE_OFFSET_BITS=64"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/lib/wx/include/osx_cocoa-unicode-3.3;${_IMPORT_PREFIX}/include/wx-3.3"
  INTERFACE_LINK_LIBRARIES "wx::wxbase"
)

# Load information for each installed configuration.
file(GLOB _cmake_config_files "${CMAKE_CURRENT_LIST_DIR}/wxWidgetsTargets-*.cmake")
foreach(_cmake_config_file IN LISTS _cmake_config_files)
  include("${_cmake_config_file}")
endforeach()
unset(_cmake_config_file)
unset(_cmake_config_files)

# Cleanup temporary variables.
set(_IMPORT_PREFIX)

# Loop over all imported files and verify that they actually exist
foreach(_cmake_target IN LISTS _cmake_import_check_targets)
  if(CMAKE_VERSION VERSION_LESS "3.28"
      OR NOT DEFINED _cmake_import_check_xcframework_for_${_cmake_target}
      OR NOT IS_DIRECTORY "${_cmake_import_check_xcframework_for_${_cmake_target}}")
    foreach(_cmake_file IN LISTS "_cmake_import_check_files_for_${_cmake_target}")
      if(NOT EXISTS "${_cmake_file}")
        message(FATAL_ERROR "The imported target \"${_cmake_target}\" references the file
   \"${_cmake_file}\"
but this file does not exist.  Possible reasons include:
* The file was deleted, renamed, or moved to another location.
* An install or uninstall procedure did not complete successfully.
* The installation package was faulty and contained
   \"${CMAKE_CURRENT_LIST_FILE}\"
but not all the files it references.
")
      endif()
    endforeach()
  endif()
  unset(_cmake_file)
  unset("_cmake_import_check_files_for_${_cmake_target}")
endforeach()
unset(_cmake_target)
unset(_cmake_import_check_targets)

# This file does not depend on other imported targets which have
# been exported from the same project but in a separate export set.

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
cmake_policy(POP)
