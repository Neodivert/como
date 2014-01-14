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

# Libraries path (GNU/Linux 32 bits).
debug_linux_32 {
    INCLUDEPATH = /opt/boost32/include/
    LIBS = -L/opt/boost32/lib/
}

# TODO: Remove
INCLUDEPATH += /opt/boost/include/
LIBS += -L/opt/boost/lib/

# Libraries
unix|win32: LIBS += -lboost_system
unix|win32: LIBS += -lboost_thread

# C++11 support (http://qt-project.org/forums/viewthread/19989)
CONFIG += c++11

# Set the target and the destination dir according to the current build in use.
# http://stackoverflow.com/questions/2580934/how-to-specify-different-debug-release-output-directories-in-qmake-pro-file
# IMPORTANT: The "debug_linux_64" is passed to CONFIG variable through
# qmake's additional arguments (project options).
debug_linux_64 {
    TARGET = client_debug_linux_64

    DESTDIR = .
    BUILD_DATA_DIR = $$DESTDIR/.build_data/debug_linux_64
}
debug_linux_32 {
    TARGET = client_debug_linux_32

    DESTDIR = .
    BUILD_DATA_DIR = $$DESTDIR/.build_data/debug_linux_32
}

OBJECTS_DIR = $$BUILD_DATA_DIR/obj
MOC_DIR = $$BUILD_DATA_DIR/moc
RCC_DIR = $$BUILD_DATA_DIR/qrc
UI_DIR = $$BUILD_DATA_DIR/ui

# C++ flags
QMAKE_CXXFLAGS_WARN_ON += -Wall -Werror
QMAKE_CXXFLAGS += -pedantic-errors

# Code headers
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
    ../../src/common/packets/user_accepted.hpp \
    ../../src/common/packets/packets.hpp \
    ../../src/common/packets/packet.hpp \
    ../../src/common/packets/packable.hpp \
    ../../src/common/packets/new_user.hpp \
    ../../src/common/utilities/packer.hpp \
    ../../src/common/packets/scene_update.hpp \
    ../../src/common/packets/scene_commands/user_connected.hpp \
    ../../src/common/packets/scene_commands/scene_commands.hpp \
    ../../src/common/packets/scene_commands/scene_command.hpp \
    ../../src/client/models/users/public_user.hpp \
    ../../src/common/utilities/log.hpp \
    ../../src/client/gui/users_list.hpp \
    ../../src/common/packets/scene_commands/drawable_commands/drawable_command.hpp \
    ../../src/common/utilities/ids.hpp \
    ../../src/client/managers/scene/public_scene.hpp \
    ../../src/common/users/basic_user.hpp \
    ../../src/common/packets/scene_commands/drawable_commands/create_cube.hpp \
    ../../src/client/managers/server_interface/server_interface.hpp \
    ../../src/common/packets/scene_commands/selection_response.hpp \
    ../../src/common/packets/scene_commands/drawable_commands/select_drawable.hpp \
    ../../src/common/packets/scene_commands/selection_transformation.hpp \
    ../../src/client/models/3d/drawables_selection.hpp \
    ../../src/common/packets/scene_commands/change_parameter.hpp

# Code sources
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
    ../../src/common/packets/user_accepted.cpp \
    ../../src/common/packets/packet.cpp \
    ../../src/common/packets/new_user.cpp \
    ../../src/common/utilities/packer.cpp \
    ../../src/common/packets/scene_update.cpp \
    ../../src/common/packets/scene_commands/user_connected.cpp \
    ../../src/common/packets/scene_commands/scene_command.cpp \
    ../../src/client/models/users/public_user.cpp \
    ../../src/client/gui/users_list.cpp \
    ../../src/common/packets/scene_commands/drawable_commands/drawable_command.cpp \
    ../../src/client/managers/scene/public_scene.cpp \
    ../../src/common/users/basic_user.cpp \
    ../../src/common/packets/scene_commands/drawable_commands/create_cube.cpp \
    ../../src/client/managers/server_interface/server_interface.cpp \
    ../../src/common/packets/scene_commands/selection_response.cpp \
    ../../src/common/packets/scene_commands/drawable_commands/select_drawable.cpp \
    ../../src/common/packets/scene_commands/selection_transformation.cpp \
    ../../src/client/models/3d/drawables_selection.cpp \
    ../../src/common/packets/scene_commands/change_parameter.cpp
