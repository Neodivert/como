TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# Include files and parameters that are common to both client and server.
include( ../common/common.pri )

# http://stackoverflow.com/questions/1119881/how-do-i-specifiy-an-object-directory-in-a-qt-project-file
# http://stackoverflow.com/questions/2580934/how-to-specify-different-debug-release-output-directories-in-qmake-pro-file
# http://qt-project.org/doc/qt-4.8/qmake-project-files.html#variables
# release: DESTDIR = build/release
#debug:   DESTDIR = ../../bin/server
DESTDIR = .

BUILD_DATA_DIR = $$DESTDIR/.build_data
OBJECTS_DIR = $$BUILD_DATA_DIR/obj
MOC_DIR = $$BUILD_DATA_DIR/moc
RCC_DIR = $$BUILD_DATA_DIR/qrc
UI_DIR = $$BUILD_DATA_DIR/ui

# Server headers
HEADERS += \
    ../../src/server/server.hpp \
    ../../src/server/public_user.hpp \
    ../../src/server/commands_historic.hpp

# Server sources
SOURCES += \
    ../../src/server/main.cpp \
    ../../src/server/server.cpp \
    ../../src/server/public_user.cpp \
    ../../src/server/commands_historic.cpp
