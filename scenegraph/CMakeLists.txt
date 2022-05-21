####################################################################################################

cmake_minimum_required(VERSION 3.16)
project(scenegraph LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

####################################################################################################

find_package(Qt6 COMPONENTS
  Core
  Gui
  Quick
  ShaderTools
)

####################################################################################################

qt_add_executable(scenegraph
    scenegraph.cpp scenegraph.h
    gridnode.cpp gridnode.h
    linenode.cpp linenode.h
    main.cpp
    noisynode.cpp noisynode.h
)
set_target_properties(scenegraph PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE

    # Prevent name clash with build subdirectory on case-insensitive file systems
    OUTPUT_NAME scenegraph-app
)
target_link_libraries(scenegraph PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Quick
)

qt_add_qml_module(scenegraph
    URI Scene
    VERSION 1.0
    QML_FILES
        main.qml
    RESOURCES
        shaders/line.frag.qsb
        shaders/line.vert.qsb
        shaders/noisy.frag.qsb
        shaders/noisy.vert.qsb
    RESOURCE_PREFIX "/scenegraph/scene"
    NO_RESOURCE_TARGET_PATH
)

qt_add_shaders(scenegraph "scenegraph_shaders"
  # Failed to find shader ":/scenegraph/scene/shaders/noisy.vert.qsb"
  # No GLSL shader code found (versions tried:  QList(460, 450, 440, 430, 420, 410, 400, 330, 150, 140, 130, 120) )
  #   in baked shader QShader(stage=0 shaders=QList(ShaderKey(1 Version(440 QFlags()) 0), ShaderKey(0 Version(100 QFlags()) 0)) desc.isValid=true)

  # qsb -b --glsl "150,120,100 es" --hlsl 50 --msl 12 -o xxx.vert.qsb xxx.vert
  GLSL "150,120,100 es"
  # GLSL "440"
  NOHLSL
  NOMSL
  # shaders/noisy.vert -> shaders/noisy.vert.qsb exposed as :/scenegraph/scene/shaders/noisy.vert.qsb
  PREFIX "/scenegraph/scene/DISABLED/"
  # PREFIX "/scenegraph/scene/"
  FILES
  shaders/noisy.vert
  shaders/noisy.frag
  shaders/line.vert
  shaders/line.frag
)
