# Install script for directory: /Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/dependency/wxWidgets/build/cmake/lib

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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/libs/base/cmake_install.cmake")
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/libs/net/cmake_install.cmake")
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/libs/core/cmake_install.cmake")
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/libs/adv/cmake_install.cmake")
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/libs/aui/cmake_install.cmake")
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/libs/html/cmake_install.cmake")
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/libs/propgrid/cmake_install.cmake")
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/libs/ribbon/cmake_install.cmake")
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/libs/richtext/cmake_install.cmake")
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/libs/webview/cmake_install.cmake")
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/libs/stc/cmake_install.cmake")
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/libs/xrc/cmake_install.cmake")
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/libs/media/cmake_install.cmake")
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/libs/gl/cmake_install.cmake")
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/libs/qa/cmake_install.cmake")
  include("/Users/shuhancao/Desktop/24fall/576/CS576_Final_Project/cmake-build-debug/dependency/wxWidgets/libs/xml/cmake_install.cmake")

endif()

