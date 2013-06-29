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
        src/gui/main_window.cpp \
    src/gui/opengl_canvas.cpp \
    src/3d_entities/geometry.cpp \
    src/3d_entities/cube.cpp \
    src/utilities/shader_loader.cpp

HEADERS  += \
    src/gui/main_window.hpp \
    src/gui/opengl_canvas.hpp \
    src/3d_entities/geometry.hpp \
    src/3d_entities/cube.hpp \
    src/3d_entities/drawable_entity.hpp \
    src/utilities/shader_loader.hpp

FORMS    += main_window.ui

LIBS += -lGLEW
