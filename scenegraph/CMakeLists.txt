cmake_minimum_required(VERSION 3.16)
project(scenegraph LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS
  Core
  Gui
  Quick
)

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
    RESOURCE_PREFIX /scenegraph/scene
    NO_RESOURCE_TARGET_PATH
)
