#-------------------------------------------------
# Project file including options and files that are common to both client and
# server.
#-------------------------------------------------

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# TODO: Remove
LIBS += -L/usr/local/lib

# Include boost headers as system headers, so they don't produce warnings.
QMAKE_CXXFLAGS += -isystem /opt/boost/include

# Common libraries
LIBS += -lpthread -lboost_system -lboost_thread -lboost_filesystem

# C++11 support (http://qt-project.org/forums/viewthread/19989)
CONFIG += c++11

# C++ flags
QMAKE_CXXFLAGS_WARN_ON += -Wall -Werror
QMAKE_CXXFLAGS += -pthread -pedantic-errors

# Common headers (used by both client and server).
HEADERS += \
    ../../src/common/utilities/packer.hpp \
    ../../src/common/packables/packable.hpp \
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
    ../../src/common/commands/drawable_commands/mesh_creation_command.hpp \
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
    ../../src/common/packables/packable_uint8_array.hpp \
    ../../src/common/packables/packable_vector4.hpp \
    ../../src/common/packets/packet_header.hpp \
    ../../src/common/commands/packable_commands_list.hpp \
    ../../src/common/packables/packable_file.hpp \
    ../../src/common/commands/primitive_commands/primitive_creation_command.hpp \
    ../../src/common/packables/packable_array.hpp \
    ../../src/common/packables/packable_integer.hpp \
    ../../src/common/commands/primitive_commands/primitive_command.hpp


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
    ../../src/common/commands/drawable_commands/mesh_creation_command.cpp \
    ../../src/common/commands/drawable_commands/drawable_selection_command.cpp \
    ../../src/common/commands/selection_commands/selection_response_command.cpp \
    ../../src/common/commands/selection_commands/selection_transformation_command.cpp \
    ../../src/common/commands/selection_commands/full_deselection_command.cpp \
    ../../src/common/commands/selection_commands/selection_command.cpp \
    ../../src/common/commands/selection_commands/selection_deletion_command.cpp \
    ../../src/common/packables/packable_vector4.cpp \
    ../../src/common/packets/packet_header.cpp \
    ../../src/common/commands/packable_commands_list.cpp \
    ../../src/common/packables/packable_file.cpp \
    ../../src/common/commands/primitive_commands/primitive_creation_command.cpp \
    ../../src/common/commands/primitive_commands/primitive_command.cpp
