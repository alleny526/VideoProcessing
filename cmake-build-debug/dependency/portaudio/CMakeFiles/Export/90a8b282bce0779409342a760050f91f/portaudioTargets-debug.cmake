#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "portaudio" for configuration "Debug"
set_property(TARGET portaudio APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(portaudio PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk/System/Library/Frameworks/CoreAudio.framework;/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk/System/Library/Frameworks/AudioToolbox.framework;/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk/System/Library/Frameworks/AudioUnit.framework;/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk/System/Library/Frameworks/CoreFoundation.framework;/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk/System/Library/Frameworks/CoreServices.framework;m;pthread"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libportaudio.dylib"
  IMPORTED_SONAME_DEBUG "@rpath/libportaudio.dylib"
  )

list(APPEND _cmake_import_check_targets portaudio )
list(APPEND _cmake_import_check_files_for_portaudio "${_IMPORT_PREFIX}/lib/libportaudio.dylib" )

# Import target "portaudio_static" for configuration "Debug"
set_property(TARGET portaudio_static APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(portaudio_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk/System/Library/Frameworks/CoreAudio.framework;/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk/System/Library/Frameworks/AudioToolbox.framework;/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk/System/Library/Frameworks/AudioUnit.framework;/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk/System/Library/Frameworks/CoreFoundation.framework;/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk/System/Library/Frameworks/CoreServices.framework;m;pthread"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libportaudio.a"
  )

list(APPEND _cmake_import_check_targets portaudio_static )
list(APPEND _cmake_import_check_files_for_portaudio_static "${_IMPORT_PREFIX}/lib/libportaudio.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
