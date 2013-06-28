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
    src/gui/opengl_canvas.cpp

HEADERS  += \
    src/gui/main_window.hpp \
    src/gui/opengl_canvas.hpp

FORMS    += main_window.ui
