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
    src/gui/opengl_canvas.cpp \
    src/3d_entities/geometry.cpp \
    src/3d_entities/cube.cpp \
    src/utilities/shader_loader.cpp \
    src/3d_entities/drawable_entity.cpp \
    src/gui/view_frame.cpp \
    src/3d_entities/scene.cpp \
    src/gui/render_panel.cpp \
    src/gui/opengl_context.cpp \
    src/gui/app_window.cpp

HEADERS  += \
    src/gui/opengl_canvas.hpp \
    src/3d_entities/geometry.hpp \
    src/3d_entities/cube.hpp \
    src/3d_entities/drawable_entity.hpp \
    src/utilities/shader_loader.hpp \
    src/gui/view_frame.hpp \
    src/3d_entities/scene.hpp \
    src/gui/render_panel.hpp \
    src/gui/opengl_context.hpp \
    src/gui/app_window.hpp

FORMS    += main_window.ui

LIBS += -lGLEW -lGLU

# http://qt-project.org/forums/viewthread/19989
CONFIG += c++11
