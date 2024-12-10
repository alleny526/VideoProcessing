cmake_minimum_required(VERSION 3.1)

project(ImageDisplay)

set(CMAKE_CXX_STANDARD 17)

# Add source files
set(PLAYERSOURCES
        manifest/MyImageApplication.rc
        src/Video2AudioPlayer.cpp
)

# Add source files
set(ENCODERSOURCES
        src/DCTCompressor.cpp
        src/encoderMain.cpp
)

# Add source files
set(DECODERSOURCES
        src/DCTCompressor.cpp
        src/decoderMain.cpp
)

if(WIN32)
    set(wxBUILD_SHARED OFF)
    # Add these lines to resolve linking conflicts
    add_definitions(-DWINVER=0x0601)
    add_definitions(-D_WIN32_WINNT=0x0601)
else()
    set(wxBUILD_SHARED ON)
endif()

# Add PortAudio
add_subdirectory(dependency/portaudio)

add_subdirectory(dependency/wxWidgets)

# Create executable
add_executable(MyImageApplication ${PLAYERSOURCES})
add_executable(MyEncoder ${ENCODERSOURCES})
add_executable(MyDecoder ${DECODERSOURCES})

# X11 required on Linux
if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
endif()

# Specific Windows libraries
if(WIN32)
    target_link_libraries(MyImageApplication
        wx::core 
        wx::base 
        portaudio_static 
        ole32
        oleaut32
        uuid
    )
else()
    # Link libraries for non-Windows platforms
    target_link_libraries(MyImageApplication
        wx::core 
        wx::base 
        portaudio_static 
        ${X11_LIBRARIES}
    )
endif()