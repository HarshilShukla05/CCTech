cmake_minimum_required(VERSION 3.16)
project(Sketcher)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)

# Qt & OpenGL
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)
find_package(OpenGL REQUIRED)

# Includes
include_directories(
    geometry/include
    geometry/src
    SketcherGUI/include
    SketcherGUI
)

# Sources
file(GLOB_RECURSE GEOM_SRC geometry/src/*.cpp)

set(GUI_SRC
    SketcherGUI/src/main.cpp
    SketcherGUI/src/mainwindow.cpp
    SketcherGUI/src/GLWidget.cpp
    SketcherGUI/src/ShapeInputDialog.cpp
    SketcherGUI/src/TransformDialogue.cpp
    SketcherGUI/src/SketchGLWidget.cpp
    SketcherGUI/src/SketchWindow.cpp
    SketcherGUI/src/polygon_boolean.cpp
    SketcherGUI/src/ExtrudeUtils.cpp
    SketcherGUI/src/clipper.cpp
    SketcherGUI/src/TriangleIntersection.cpp
)

set(GUI_HEADERS
    SketcherGUI/include/mainwindow.h
    SketcherGUI/include/GLWidget.h
    SketcherGUI/include/ShapeInputDialog.h
    SketcherGUI/include/TransformDialogue.h
    SketcherGUI/include/SketchGLWidget.h
    SketcherGUI/include/SketchWindow.h
    SketcherGUI/include/polygon_boolean.h
    SketcherGUI/include/ExtrudeUtils.h
    SketcherGUI/include/clipper.hpp
    SketcherGUI/include/TriangleIntersection.h
)

set(GUI_UI
    SketcherGUI/src/mainwindow.ui
)

add_executable(${PROJECT_NAME}
    ${GEOM_SRC}
    ${GUI_SRC}
    ${GUI_HEADERS}
    ${GUI_UI}
)

target_link_libraries(${PROJECT_NAME}
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    OpenGL::GL
    glu32
)