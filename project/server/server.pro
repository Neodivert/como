TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# Include files and parameters that are common to both client and server.
include( ../common/common.pri )

# Set the target and the destination dir according to the current build in use.
# http://stackoverflow.com/questions/2580934/how-to-specify-different-debug-release-output-directories-in-qmake-pro-file
DESTDIR = .

CONFIG( debug, debug|release ) {
    TARGET = server_debug
} else {
    TARGET = server
}
message( Building target: $$TARGET )

BUILD_DATA_DIR = $$DESTDIR/.build_data/$$TARGET
OBJECTS_DIR = $$BUILD_DATA_DIR/obj
MOC_DIR = $$BUILD_DATA_DIR/moc
RCC_DIR = $$BUILD_DATA_DIR/qrc
UI_DIR = $$BUILD_DATA_DIR/ui

INCLUDEPATH += ../../src

# Server headers
HEADERS += \
    ../../src/server/server.hpp \
    ../../src/server/public_user.hpp \
    ../../src/server/commands_historic.hpp \
    ../../src/server/managers/server_primitives_manager.hpp \
    ../../src/server/sync_data/resource_sync_data.hpp \
    ../../src/server/sync_data/sync_data.hpp \
    ../../src/server/sync_data/texture_sync_data.hpp \
    ../../src/server/managers/resources_synchronization_library.hpp \
    ../../src/server/managers/scene.hpp \
    ../../src/server/sync_data/entity_sync_data.hpp \
    ../../src/server/sync_data/texture_wall_sync_data.hpp \
    ../../src/server/sync_data/material_sync_data.hpp \
    ../../src/server/sync_data/camera_sync_data.hpp \
    ../../src/server/sync_data/light_sync_data.hpp

# Server sources
SOURCES += \
    ../../src/server/main.cpp \
    ../../src/server/server.cpp \
    ../../src/server/public_user.cpp \
    ../../src/server/commands_historic.cpp \
    ../../src/server/managers/server_primitives_manager.cpp \
    ../../src/server/sync_data/sync_data.cpp \
    ../../src/server/sync_data/resource_sync_data.cpp \
    ../../src/server/managers/resources_synchronization_library.cpp \
    ../../src/server/managers/scene.cpp \
    ../../src/server/sync_data/entity_sync_data.cpp \
    ../../src/server/sync_data/texture_wall_sync_data.cpp \
    ../../src/server/sync_data/material_sync_data.cpp \
    ../../src/server/sync_data/camera_sync_data.cpp \
    ../../src/server/sync_data/light_sync_data.cpp
