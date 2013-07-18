#-------------------------------------------------
#
# Project created by QtCreator 2013-06-28T12:00:12
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = como_project
TEMPLATE = app


SOURCES += src/main.cpp\
    src/gui/opengl_engine.cpp \
    src/gui/tools_menu.cpp \
    src/gui/opengl_canvas.cpp \
    src/gui/render_panel.cpp \
    src/gui/main_window.cpp \
    src/gui/view_frame.cpp \
    src/managers/como_app.cpp \
    src/managers/scene.cpp \
    src/managers/shader_loader.cpp \
    src/models/3d/geometry.cpp \
    src/models/3d/cube.cpp \
    src/models/3d/camera.cpp \
    src/models/3d/drawable.cpp


HEADERS  += \
    src/gui/opengl_engine.hpp \
    src/gui/tools_menu.hpp \
    src/gui/opengl_canvas.hpp \
    src/gui/render_panel.hpp \
    src/gui/main_window.hpp \
    src/gui/view_frame.hpp \
    src/managers/como_app.hpp \
    src/managers/scene.hpp \
    src/managers/shader_loader.hpp \
    src/models/3d/geometry.hpp \
    src/models/3d/cube.hpp \
    src/models/3d/camera.hpp \
    src/models/3d/drawable.hpp

FORMS    += main_window.ui

LIBS += -lGLEW -lGLU

# http://qt-project.org/forums/viewthread/19989
CONFIG += c++11
