# Install script for directory: /Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/dependency/wxWidgets

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  message(STATUS "Installing: Headers...")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/wx-3.3" TYPE DIRECTORY FILES "/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/dependency/wxWidgets/include/wx")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/wx/include" TYPE DIRECTORY FILES "/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/lib/wx/include/osx_cocoa-unicode-3.3")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/wx/config" TYPE FILE PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ FILES "/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/lib/wx/config/osx_cocoa-unicode-3.3")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE DIRECTORY FILES "")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(         COMMAND /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake -E create_symlink         "/usr/local/lib/wx/config/osx_cocoa-unicode-3.3"         "$ENV{DESTDIR}/usr/local/bin/wx-config"         )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/wxWidgets/wxWidgetsTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/wxWidgets/wxWidgetsTargets.cmake"
         "/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/CMakeFiles/Export/82dea25457d57761b815f3a88ce12492/wxWidgetsTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/wxWidgets/wxWidgetsTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/wxWidgets/wxWidgetsTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/wxWidgets" TYPE FILE FILES "/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/CMakeFiles/Export/82dea25457d57761b815f3a88ce12492/wxWidgetsTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/wxWidgets" TYPE FILE FILES "/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/CMakeFiles/Export/82dea25457d57761b815f3a88ce12492/wxWidgetsTargets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/wxWidgets" TYPE FILE FILES
    "/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/lib/wxWidgetsConfig.cmake"
    "/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/lib/wxWidgetsConfigVersion.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/libs/cmake_install.cmake")
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/utils/cmake_install.cmake")

endif()

