# CMake configuration for the drum machine project.
# Declares sources, include paths and platform-specific link options.

cmake_minimum_required(VERSION 3.20)

project(beatbox_64464)

set(CMAKE_CXX_STANDARD 20)          # Use C++20 for modern features.
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Fail if the compiler does not support it.
set(CMAKE_CXX_EXTENSIONS OFF)       # Disable compiler-specific extensions.

# Header search paths.
include_directories(
    "src"
    "libraries/imgui/backends"
    "libraries/imgui"
    "libraries/portaudio/include"
    "libraries/sdl/include"
)

# Every source file compiled into the final executable.
add_executable(beatbox_64464
    main.cpp
    src/ui/MainWindow.cpp
    src/ui/MainWindowDraw.cpp
    src/ui/MainWindowInspector.cpp
    src/ui/MainWindowSequencer.cpp
    src/core/DrumMachine.cpp
    src/core/Preferences.cpp
    src/audio/AudioEngine.cpp
    src/audio/AudioFileReader.cpp
    src/audio/AudioPlayer.cpp
    src/audio/Delay.cpp
    src/audio/Mixer.cpp
    src/audio/StepSequencer.cpp
    libraries/imgui/imgui.cpp
    libraries/imgui/imgui_demo.cpp
    libraries/imgui/imgui_draw.cpp
    libraries/imgui/imgui_widgets.cpp
    libraries/imgui/imgui_tables.cpp
    libraries/imgui/backends/imgui_impl_sdl3.cpp
    libraries/imgui/backends/imgui_impl_sdlrenderer3.cpp
)

if (APPLE)
    # Tell the executable where to find SDL3.framework at runtime.
    set(CMAKE_INSTALL_RPATH "@executable_path/../libraries/sdl/lib/macos/SDL3.framework")
    target_link_libraries(beatbox_64464 PRIVATE
        "${CMAKE_SOURCE_DIR}/libraries/sdl/lib/macos/SDL3.framework"
        "-framework AudioToolbox"
        "-framework CoreAudio"
        "-framework Carbon"
        "${CMAKE_SOURCE_DIR}/libraries/portaudio/lib/macos/libportaudio.a"
    )
elseif (WIN32)
    # MinGW (used by run.bat through MSYS2) can link directly against
    # DLLs, so we keep that approach here.
    target_link_libraries(beatbox_64464 PRIVATE
        "${CMAKE_SOURCE_DIR}/libraries/sdl/lib/win32/sdl3.dll"
        "${CMAKE_SOURCE_DIR}/libraries/portaudio/lib/win32/portaudio_x64.dll"
    )

    # Copy the DLLs next to the executable so they can be loaded at runtime.
    add_custom_command(TARGET beatbox_64464 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/libraries/sdl/lib/win32/sdl3.dll"
        "$<TARGET_FILE_DIR:beatbox_64464>"
    )

    add_custom_command(TARGET beatbox_64464 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/libraries/portaudio/lib/win32/portaudio_x64.dll"
        "$<TARGET_FILE_DIR:beatbox_64464>"
    )
elseif (UNIX)
    target_link_libraries(beatbox_64464 PRIVATE
        "-lGL"
        "-ldl"
        "-ljack"
        "${CMAKE_SOURCE_DIR}/libraries/sdl/lib/linux-x86_64/libSDL3.a"
        "${CMAKE_SOURCE_DIR}/libraries/portaudio/lib/linux-x86_64/libportaudio.a"
    )
endif()
