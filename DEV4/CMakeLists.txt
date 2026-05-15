# Le CMakeList il sert a configurer le projet, a dire ou sont les fichiers sources, les bibliotheques, etc. 

cmake_minimum_required(VERSION 3.20)

project(beatbox_64464)

set(CMAKE_CXX_STANDARD 20) # On utilise le C++20 pour les features modernes

include_directories( # On dis de chercher les fichiers .h dans ces dossiers
    "src"
    "libraries/imgui/backends"
    "libraries/imgui"
    "libraries/portaudio/include"
    "libraries/sdl/include"
)

add_executable(beatbox_64464 # Prends touts les fichiers . cpp pour les compiler
    main.cpp
    src/ui/MainWindow.cpp
    src/ui/MainWindowDraw.cpp
    src/ui/MainWindowInspector.cpp
    src/ui/MainWindowSequencer.cpp
    src/core/DrumMachine.cpp
    src/core/Preferences.cpp
    src/audio/AudioEngine.cpp
    src/audio/AudioFileReader.cpp
    src/audio/AudioGenerator.cpp
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
# Quand le programme tourne on lui dis ou chercher SDL3.framework, et les autres bibliotheques necessaires pour le son
    set(CMAKE_INSTALL_RPATH "@executable_path/../libraries/sdl/lib/macos/SDL3.framework")
    # On va lier le projet avec les bibliotheques SDL3, AudioToolbox, CoreAudio, Carbon et PortAudio. Les chemins sont differents selon le systeme d'exploitation
    target_link_libraries(beatbox_64464 PRIVATE
        "${CMAKE_SOURCE_DIR}/libraries/sdl/lib/macos/SDL3.framework"
        "-framework AudioToolbox"
        "-framework CoreAudio"
        "-framework Carbon"
        "${CMAKE_SOURCE_DIR}/libraries/portaudio/lib/macos/libportaudio.a"
    )
elseif (WIN32)
    target_link_libraries(beatbox_64464 PRIVATE
        "${CMAKE_SOURCE_DIR}/libraries/sdl/lib/win32/sdl3.dll"
        "${CMAKE_SOURCE_DIR}/libraries/portaudio/lib/win32/portaudio_x64.dll"
    )
    # Sur Mac , les bibliothèques sont intégrées dans le programme 
    # Sur Windows , on doit copier les fichiers .dll dans le dossier de sortie pour que le programme puisse les trouver quand il s'exécute. Ces commandes font ça après la compilation du projet.
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
