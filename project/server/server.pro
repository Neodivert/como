TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# Link boost libraries
LIBS += -lpthread -lGL
unix|win32: LIBS += -lboost_system -lboost_thread -lboost_filesystem

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

# Common headers (used by both client and server).
HEADERS += \
    ../../src/common/utilities/packer.hpp \
    ../../src/common/packables/packable.hpp \
    ../../src/common/packables/packable_integer.hpp \
    ../../src/common/packables/composite_packable.hpp \
    ../../src/common/packets/packet.hpp \
    ../../src/common/packets/packets.hpp \
    ../../src/common/packets/new_user_packet.hpp \
    ../../src/common/packets/user_acceptance_packet.hpp \
    ../../src/common/packets/scene_update_packet.hpp \
    ../../src/common/utilities/ids.hpp \
    ../../src/common/utilities/log.hpp \
    ../../src/common/users/basic_user.hpp \
    ../../src/common/commands/command.hpp \
    ../../src/common/commands/commands.hpp \
    ../../src/common/commands/user_commands/user_command.hpp \
    ../../src/common/commands/user_commands/user_connection_command.hpp \
    ../../src/common/commands/user_commands/user_disconnection_command.hpp \
    ../../src/common/commands/user_commands/parameter_change_command.hpp \
    ../../src/common/commands/drawable_commands/drawable_command.hpp \
    ../../src/common/commands/drawable_commands/cube_creation_command.hpp \
    ../../src/common/commands/drawable_commands/drawable_selection_command.hpp \
    ../../src/common/commands/selection_commands/selection_response_command.hpp \
    ../../src/common/commands/selection_commands/selection_transformation_command.hpp \
    ../../src/common/commands/selection_commands/full_deselection_command.hpp \
    ../../src/common/commands/selection_commands/selection_command.hpp \
    ../../src/common/commands/selection_commands/selection_deletion_command.hpp \
    ../../src/common/packables/packables.hpp \
    ../../src/common/packables/packable_uint8.hpp \
    ../../src/common/packables/packable_uint16.hpp \
    ../../src/common/packables/packable_uint32.hpp \
    ../../src/common/packables/packable_string.hpp \
    ../../src/common/packables/packable_array.hpp \
    ../../src/common/packables/packable_uint8_array.hpp \
    ../../src/common/packables/packable_vector4.hpp \
    ../../src/common/packets/packet_header.hpp \
    ../../src/common/commands/packable_commands_list.hpp

# Server sources
SOURCES += \
    ../../src/server/main.cpp \
    ../../src/server/server.cpp \
    ../../src/server/public_user.cpp \
    ../../src/server/commands_historic.cpp

# Common sources (used by both client and server).
SOURCES += \
    ../../src/common/utilities/packer.cpp \
    ../../src/common/packables/composite_packable.cpp \
    ../../src/common/packets/packet.cpp \
    ../../src/common/packets/new_user_packet.cpp \
    ../../src/common/packets/user_acceptance_packet.cpp \
    ../../src/common/packets/scene_update_packet.cpp \
    ../../src/common/utilities/ids.cpp \
    ../../src/common/users/basic_user.cpp \
    ../../src/common/commands/command.cpp \
    ../../src/common/commands/user_commands/user_command.cpp \
    ../../src/common/commands/user_commands/user_connection_command.cpp \
    ../../src/common/commands/user_commands/user_disconnection_command.cpp \
    ../../src/common/commands/user_commands/parameter_change_command.cpp \
    ../../src/common/commands/drawable_commands/drawable_command.cpp \
    ../../src/common/commands/drawable_commands/cube_creation_command.cpp \
    ../../src/common/commands/drawable_commands/drawable_selection_command.cpp \
    ../../src/common/commands/selection_commands/selection_response_command.cpp \
    ../../src/common/commands/selection_commands/selection_transformation_command.cpp \
    ../../src/common/commands/selection_commands/full_deselection_command.cpp \
    ../../src/common/commands/selection_commands/selection_command.cpp \
    ../../src/common/commands/selection_commands/selection_deletion_command.cpp \
    ../../src/common/packables/packable_vector4.cpp \
    ../../src/common/packets/packet_header.cpp \
    ../../src/common/commands/packable_commands_list.cpp
