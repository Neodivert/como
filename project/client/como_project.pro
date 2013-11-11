#-------------------------------------------------
#
# Project created by QtCreator 2013-06-28T12:00:12
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = como_project
TEMPLATE = app

FORMS    += main_window.ui

LIBS += -lGLEW -lGLU

# http://qt-project.org/forums/viewthread/19989
CONFIG += c++11

QMAKE_CXXFLAGS_WARN_ON += -Wall -Werror
QMAKE_CXXFLAGS += -pedantic-errors

unix|win32: LIBS += -lboost_system
unix|win32: LIBS += -lboost_thread

HEADERS += \
    ../../src/client/gui/viewport.hpp \
    ../../src/client/gui/view_frame.hpp \
    ../../src/client/gui/tools_menu.hpp \
    ../../src/client/gui/render_panel.hpp \
    ../../src/client/gui/opengl_engine.hpp \
    ../../src/client/gui/main_window.hpp \
    ../../src/client/managers/tester.hpp \
    ../../src/client/managers/scene.hpp \
    ../../src/client/managers/como_app.hpp \
    ../../src/client/models/3d/mesh.hpp \
    ../../src/client/models/3d/drawable.hpp \
    ../../src/client/models/3d/cube.hpp \
    ../../src/client/models/3d/camera.hpp \
    ../../src/client/models/utilities/msl/src/shader_loader.hpp \
    ../../src/client/models/server/server_interface.hpp \
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
    ../../src/common/utilities/log.hpp

SOURCES += \
    ../../src/client/gui/viewport.cpp \
    ../../src/client/gui/view_frame.cpp \
    ../../src/client/gui/tools_menu.cpp \
    ../../src/client/gui/render_panel.cpp \
    ../../src/client/gui/opengl_engine.cpp \
    ../../src/client/gui/main_window.cpp \
    ../../src/client/managers/tester.cpp \
    ../../src/client/managers/scene.cpp \
    ../../src/client/managers/como_app.cpp \
    ../../src/client/models/3d/mesh.cpp \
    ../../src/client/models/3d/drawable.cpp \
    ../../src/client/models/3d/cube.cpp \
    ../../src/client/models/3d/camera.cpp \
    ../../src/client/models/utilities/msl/src/shader_loader.cpp \
    ../../src/client/main.cpp \
    ../../src/client/models/server/server_interface.cpp \
    ../../src/client/gui/connection_wizard/connection_wizard.cpp \
    ../../src/client/gui/connection_wizard/connect_server_page.cpp \
    ../../src/client/gui/connection_wizard/create_server_page.cpp \
    ../../src/client/gui/connection_wizard/intro_page.cpp \
    ../../src/common/packets/user_accepted.cpp \
    ../../src/common/packets/packet.cpp \
    ../../src/common/packets/packable.cpp \
    ../../src/common/packets/new_user.cpp \
    ../../src/common/utilities/packer.cpp \
    ../../src/common/packets/scene_update.cpp \
    ../../src/common/packets/scene_commands/user_connected.cpp \
    ../../src/common/packets/scene_commands/scene_command.cpp
