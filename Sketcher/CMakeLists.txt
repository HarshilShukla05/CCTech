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
    SketcherGUI
)

# Sources
file(GLOB_RECURSE GEOM_SRC geometry/src/*.cpp)

set(GUI_SRC
    SketcherGUI/main.cpp
    SketcherGUI/mainwindow.cpp
    SketcherGUI/GLWidget.cpp
    SketcherGUI/ShapeInputDialog.cpp
    SketcherGUI/TransformDialogue.cpp
)

set(GUI_HEADERS
    SketcherGUI/mainwindow.h
    SketcherGUI/GLWidget.h
    SketcherGUI/ShapeInputDialog.h
    SketcherGUI/TransformDialogue.h
)

set(GUI_UI
    SketcherGUI/mainwindow.ui
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
)