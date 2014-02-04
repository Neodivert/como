TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# Link boost libraries
LIBS += -lpthread -lGL
unix|win32: LIBS += -lboost_system
unix|win32: LIBS += -lboost_thread

# TODO: Remove
LIBS += -L/opt/boost/lib

# Include boost headers as system headers, so they don't produce warnings.
QMAKE_CXXFLAGS += -isystem /opt/boost/include

# http://qt-project.org/forums/viewthread/19989
CONFIG += c++11

# Compilation flags.
QMAKE_CXXFLAGS_WARN_ON += -Wall -Werror
QMAKE_CXXFLAGS += -pedantic-errors

# http://stackoverflow.com/questions/1119881/how-do-i-specifiy-an-object-directory-in-a-qt-project-file
# http://stackoverflow.com/questions/2580934/how-to-specify-different-debug-release-output-directories-in-qmake-pro-file
# http://qt-project.org/doc/qt-4.8/qmake-project-files.html#variables
# release: DESTDIR = build/release
debug:   DESTDIR = ../../bin/server

BUILD_DATA_DIR = $$DESTDIR/.build_data
OBJECTS_DIR = $$BUILD_DATA_DIR/obj
MOC_DIR = $$BUILD_DATA_DIR/moc
RCC_DIR = $$BUILD_DATA_DIR/qrc
UI_DIR = $$BUILD_DATA_DIR/ui

SOURCES += \
    ../../src/server/main.cpp \
    ../../src/server/server.cpp \
    ../../src/common/packets/user_acceptance_packet.cpp \
    ../../src/common/packets/packet.cpp \
    ../../src/common/packets/new_user_packet.cpp \
    ../../src/common/packets/scene_update_packet.cpp \
    ../../src/common/packets/scene_commands/scene_command.cpp \
    ../../src/common/utilities/packer.cpp \
    ../../src/common/packets/scene_commands/user_connection_command.cpp \
    ../../src/server/public_user.cpp \
    ../../src/server/commands_historic.cpp \
    ../../src/common/utilities/ids.cpp \
    ../../src/common/users/basic_user.cpp \
    ../../src/common/packets/scene_commands/drawable_commands/drawable_command.cpp \
    ../../src/common/packets/scene_commands/drawable_commands/cube_creation_command.cpp \
    ../../src/common/packets/scene_commands/selection_response_command.cpp \
    ../../src/common/packets/scene_commands/drawable_commands/drawable_selection_command.cpp \
    ../../src/common/packets/scene_commands/selection_transformation_command.cpp \
    ../../src/common/packets/scene_commands/parameter_change_command.cpp

HEADERS += \
    ../../src/server/server.hpp \
    ../../src/common/packets/user_acceptance_packet.hpp \
    ../../src/common/packets/packet.hpp \
    ../../src/common/packets/packable.hpp \
    ../../src/common/packets/new_user_packet.hpp \
    ../../src/common/packets/packets.hpp \
    ../../src/common/packets/scene_update_packet.hpp \
    ../../src/common/packets/scene_commands/scene_command.hpp \
    ../../src/common/packets/scene_commands/scene_commands.hpp \
    ../../src/common/utilities/packer.hpp \
    ../../src/common/packets/scene_commands/user_connection_command.hpp \
    ../../src/server/public_user.hpp \
    ../../src/common/utilities/log.hpp \
    ../../src/server/commands_historic.hpp \
    ../../src/common/utilities/ids.hpp \
    ../../src/common/users/basic_user.hpp \
    ../../src/common/packets/scene_commands/drawable_commands/drawable_command.hpp \
    ../../src/common/packets/scene_commands/drawable_commands/cube_creation_command.hpp \
    ../../src/common/packets/scene_commands/selection_response_command.hpp \
    ../../src/common/packets/scene_commands/drawable_commands/drawable_selection_command.hpp \
    ../../src/common/packets/scene_commands/selection_transformation_command.hpp \
    ../../src/common/packets/scene_commands/parameter_change_command.hpp
