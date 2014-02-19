#-------------------------------------------------
#
# Project created by QtCreator 2013-06-28T12:00:12
#
#-------------------------------------------------

# Qt modules
QT       += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Template
TEMPLATE = app

# TODO: Remove
LIBS += -L/opt/boost/lib

# Include boost headers as system headers, so they don't produce warnings.
QMAKE_CXXFLAGS += -isystem /opt/boost/include

# Libraries
LIBS += -lpthread
unix|win32: LIBS += -lboost_system
unix|win32: LIBS += -lboost_thread

# C++11 support (http://qt-project.org/forums/viewthread/19989)
CONFIG += c++11

# Set the target and the destination dir according to the current build in use.
# http://stackoverflow.com/questions/2580934/how-to-specify-different-debug-release-output-directories-in-qmake-pro-file
DESTDIR = .

CONFIG( debug, debug|release ) {
    TARGET = client_debug
} else {
    TARGET = client_release
}
message( Building target: $$TARGET )

BUILD_DATA_DIR = $$DESTDIR/.build_data/$$TARGET
OBJECTS_DIR = $$BUILD_DATA_DIR/obj
MOC_DIR = $$BUILD_DATA_DIR/moc
RCC_DIR = $$BUILD_DATA_DIR/qrc
UI_DIR = $$BUILD_DATA_DIR/ui

# C++ flags
QMAKE_CXXFLAGS_WARN_ON += -Wall -Werror
QMAKE_CXXFLAGS += -pthread -pedantic-errors

# Cliet headers
HEADERS += \
    ../../src/client/gui/viewport.hpp \
    ../../src/client/gui/view_frame.hpp \
    ../../src/client/gui/tools_menu.hpp \
    ../../src/client/gui/render_panel.hpp \
    ../../src/client/gui/main_window.hpp \
    ../../src/client/managers/tester.hpp \
    ../../src/client/managers/scene/scene.hpp \
    ../../src/client/managers/como_app.hpp \
    ../../src/client/models/3d/mesh.hpp \
    ../../src/client/models/3d/drawable.hpp \
    ../../src/client/models/3d/cube.hpp \
    ../../src/client/models/3d/camera.hpp \
    ../../src/client/models/utilities/msl/src/shader_loader.hpp \
    ../../src/client/gui/connection_wizard/connection_wizard.hpp \
    ../../src/client/gui/connection_wizard/connect_server_page.hpp \
    ../../src/client/gui/connection_wizard/create_server_page.hpp \
    ../../src/client/gui/connection_wizard/intro_page.hpp \
    ../../src/client/gui/connection_wizard/page_ids.hpp \
    ../../src/client/models/users/public_user.hpp \
    ../../src/client/gui/users_list.hpp \
    ../../src/client/managers/scene/public_scene.hpp \
    ../../src/client/managers/server_interface/server_interface.hpp \
    ../../src/client/models/3d/drawables_selection.hpp \
    ../../src/common/packables/packable_wrapper.hpp



# Common headers (used by both client and server).
HEADERS += \
    ../../src/common/utilities/packer.hpp \
    ../../src/common/packables/packable.hpp \
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
    ../../src/common/commands/selection_commands/selection_deletion_command.hpp



# Client sources
SOURCES += \
    ../../src/client/gui/viewport.cpp \
    ../../src/client/gui/view_frame.cpp \
    ../../src/client/gui/tools_menu.cpp \
    ../../src/client/gui/render_panel.cpp \
    ../../src/client/gui/main_window.cpp \
    ../../src/client/managers/tester.cpp \
    ../../src/client/managers/scene/scene.cpp \
    ../../src/client/managers/como_app.cpp \
    ../../src/client/models/3d/mesh.cpp \
    ../../src/client/models/3d/drawable.cpp \
    ../../src/client/models/3d/cube.cpp \
    ../../src/client/models/3d/camera.cpp \
    ../../src/client/models/utilities/msl/src/shader_loader.cpp \
    ../../src/client/main.cpp \
    ../../src/client/gui/connection_wizard/connection_wizard.cpp \
    ../../src/client/gui/connection_wizard/connect_server_page.cpp \
    ../../src/client/gui/connection_wizard/create_server_page.cpp \
    ../../src/client/gui/connection_wizard/intro_page.cpp \
    ../../src/client/models/users/public_user.cpp \
    ../../src/client/gui/users_list.cpp \
    ../../src/client/managers/scene/public_scene.cpp \
    ../../src/client/managers/server_interface/server_interface.cpp \
    ../../src/client/models/3d/drawables_selection.cpp \


# Common sources (used by both client and server).
SOURCES += \
    ../../src/common/utilities/packer.cpp \
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
    ../../src/common/commands/selection_commands/selection_deletion_command.cpp
