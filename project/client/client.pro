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

INCLUDEPATH += ../../src

# Include files and parameters that are common to both client and server.
include( ../common/common.pri )

# Cliet headers
HEADERS += \
    ../../src/client/gui/viewport.hpp \
    ../../src/client/gui/view_frame.hpp \
    ../../src/client/gui/tools_menu/tools_menu.hpp \
    ../../src/client/gui/render_panel.hpp \
    ../../src/client/gui/main_window.hpp \
    ../../src/client/managers/scene/scene.hpp \
    ../../src/client/managers/como_app.hpp \
    ../../src/client/models/3d/mesh.hpp \
    ../../src/client/models/3d/drawable.hpp \
    ../../src/client/models/3d/camera.hpp \
    ../../src/client/models/utilities/msl/src/shader_loader.hpp \
    ../../src/client/gui/connection_wizard/connection_wizard.hpp \
    ../../src/client/gui/connection_wizard/connect_server_page.hpp \
    ../../src/client/gui/connection_wizard/create_server_page.hpp \
    ../../src/client/gui/connection_wizard/intro_page.hpp \
    ../../src/client/gui/connection_wizard/page_ids.hpp \
    ../../src/client/gui/users_list.hpp \
    ../../src/client/managers/server_interface/server_interface.hpp \
    ../../src/client/managers/drawables_selection/drawables_selection.hpp \
    ../../src/client/models/3d/lights/directional_light.hpp \
    ../../src/client/models/3d/lights/lights.hpp \
    ../../src/client/gui/tools_menu/creation_tab.hpp \
    ../../src/client/gui/tools_menu/transformation_tab.hpp \
    ../../src/client/gui/tools_menu/properties_tab/properties_tab.hpp \
    ../../src/client/gui/tools_menu/properties_tab/general_info_menu.hpp \
    ../../src/client/gui/tools_menu/properties_tab/mesh_info_menu.hpp \
    ../../src/client/models/utilities/changeable/changeable.hpp \
    ../../src/client/managers/drawables_selection/local_drawables_selection.hpp \
    ../../src/client/managers/drawables_manager/drawables_manager.hpp \
    ../../src/client/models/utilities/changeable/abstract_changeable.hpp \
    ../../src/client/models/3d/materials/material.hpp \
    ../../src/client/gui/utilities/color_button.hpp \
    ../../src/client/managers/materials_manager/materials_manager.hpp \
    ../../src/client/gui/materials/materials_editor.hpp \
    ../../src/client/gui/materials/materials_list.hpp \
    ../../src/client/gui/materials/material_panel.hpp \
    ../../src/client/gui/materials/materials_list_item.hpp \
    ../../src/client/managers/material_handler.hpp \
    ../../src/client/gui/lights/lights_editor.hpp \
    ../../src/client/gui/lights/lights_list_item.hpp \
    ../../src/client/gui/lights/lights_list.hpp \
    ../../src/client/managers/lights/lights_manager.hpp \
    ../../src/client/gui/lights/light_panel.hpp \
    ../../src/client/managers/lights/light_handler.hpp \
    ../../src/client/models/3d/lights/abstract_light.hpp \
    ../../src/client/models/3d/lights/mesh_light.hpp


# Client sources
SOURCES += \
    ../../src/client/gui/viewport.cpp \
    ../../src/client/gui/view_frame.cpp \
    ../../src/client/gui/tools_menu/tools_menu.cpp \
    ../../src/client/gui/render_panel.cpp \
    ../../src/client/gui/main_window.cpp \
    ../../src/client/managers/scene/scene.cpp \
    ../../src/client/managers/como_app.cpp \
    ../../src/client/models/3d/mesh.cpp \
    ../../src/client/models/3d/drawable.cpp \
    ../../src/client/models/3d/camera.cpp \
    ../../src/client/models/utilities/msl/src/shader_loader.cpp \
    ../../src/client/main.cpp \
    ../../src/client/gui/connection_wizard/connection_wizard.cpp \
    ../../src/client/gui/connection_wizard/connect_server_page.cpp \
    ../../src/client/gui/connection_wizard/create_server_page.cpp \
    ../../src/client/gui/connection_wizard/intro_page.cpp \
    ../../src/client/gui/users_list.cpp \
    ../../src/client/managers/server_interface/server_interface.cpp \
    ../../src/client/managers/drawables_selection/drawables_selection.cpp \
    ../../src/client/models/3d/lights/directional_light.cpp \
    ../../src/client/gui/tools_menu/creation_tab.cpp \
    ../../src/client/gui/tools_menu/transformation_tab.cpp \
    ../../src/client/gui/tools_menu/properties_tab/properties_tab.cpp \
    ../../src/client/gui/tools_menu/properties_tab/general_info_menu.cpp \
    ../../src/client/gui/tools_menu/properties_tab/mesh_info_menu.cpp \
    ../../src/client/models/utilities/changeable/changeable.cpp \
    ../../src/client/managers/drawables_selection/local_drawables_selection.cpp \
    ../../src/client/managers/drawables_manager/drawables_manager.cpp \
    ../../src/client/models/3d/materials/material.cpp \
    ../../src/client/gui/utilities/color_button.cpp \
    ../../src/client/managers/materials_manager/materials_manager.cpp \
    ../../src/client/gui/materials/materials_editor.cpp \
    ../../src/client/gui/materials/materials_list.cpp \
    ../../src/client/gui/materials/material_panel.cpp \
    ../../src/client/gui/materials/materials_list_item.cpp \
    ../../src/client/managers/material_handler.cpp \
    ../../src/client/gui/lights/lights_editor.cpp \
    ../../src/client/gui/lights/lights_list_item.cpp \
    ../../src/client/gui/lights/lights_list.cpp \
    ../../src/client/managers/lights/lights_manager.cpp \
    ../../src/client/gui/lights/light_panel.cpp \
    ../../src/client/managers/lights/light_handler.cpp \
    ../../src/client/models/3d/lights/mesh_light.cpp
