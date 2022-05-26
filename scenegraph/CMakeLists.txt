####################################################################################################
#
# To get full output use
#   make VERBOSE=1
#
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

include_directories(${Qt6Quick_PRIVATE_INCLUDE_DIRS})
# include_directories(${Qt${QT_VERSION_MAJOR}Quick_PRIVATE_INCLUDE_DIRS})
message(STATUS "Qt private API include dir: ${Qt6Quick_PRIVATE_INCLUDE_DIRS}")
# -- Qt private API include dir: /opt/Qt/6.3.0/gcc_64/include/QtQuick/6.3.0;/opt/Qt/6.3.0/gcc_64/include/QtQuick/6.3.0/QtQuick;/opt/Qt/6.3.0/gcc_64/include/QtCore/6.3.0;/opt/Qt/6.3.0/gcc_64/include/QtCore/6.3.0/QtCore;/opt/Qt/6.3.0/gcc_64/include/QtGui/6.3.0;/opt/Qt/6.3.0/gcc_64/include/QtGui/6.3.0/QtGui;/opt/Qt/6.3.0/gcc_64/include/QtDBus/6.3.0;/opt/Qt/6.3.0/gcc_64/include/QtDBus/6.3.0/QtDBus;/opt/Qt/6.3.0/gcc_64/include/QtQml/6.3.0;/opt/Qt/6.3.0/gcc_64/include/QtQml/6.3.0/QtQml;/opt/Qt/6.3.0/gcc_64/include/QtNetwork/6.3.0;/opt/Qt/6.3.0/gcc_64/include/QtNetwork/6.3.0/QtNetwork;/opt/Qt/6.3.0/gcc_64/include/QtQmlModels/6.3.0;/opt/Qt/6.3.0/gcc_64/include/QtQmlModels/6.3.0/QtQmlModels;/opt/Qt/6.3.0/gcc_64/include/QtOpenGL/6.3.0;/opt/Qt/6.3.0/gcc_64/include/QtOpenGL/6.3.0/QtOpenGL

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
        # shaders/line.frag.qsb
        # shaders/line.vert.qsb
        # shaders/noisy.frag.qsb
        # shaders/noisy.vert.qsb
        icons/qt-logo.png
    RESOURCE_PREFIX "/scenegraph/scene"
    NO_RESOURCE_TARGET_PATH
)

qt_add_shaders(scenegraph "scenegraph_shaders"
  # qsb -b --glsl "150,120,100 es" --hlsl 50 --msl 12 -o xxx.vert.qsb xxx.vert
  BATCHABLE
  GLSL "460,150,120,100 es"
  NOHLSL
  NOMSL
  # shaders/noisy.vert -> shaders/noisy.vert.qsb exposed as :/scenegraph/scene/shaders/noisy.vert.qsb
  PREFIX "/scenegraph/scene/"
  FILES
  shaders/noisy.vert
  shaders/noisy.frag
  shaders/line.vert
  shaders/line.frag
)
