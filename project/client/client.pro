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
    TARGET = client
}
message( Building target: $$TARGET )

BUILD_DATA_DIR = $$DESTDIR/.build_data/$$TARGET
OBJECTS_DIR = $$BUILD_DATA_DIR/obj
MOC_DIR = $$BUILD_DATA_DIR/moc
RCC_DIR = $$BUILD_DATA_DIR/qrc
UI_DIR = $$BUILD_DATA_DIR/ui

INCLUDEPATH += ../../src

LIBS += -lSDL2 -lSDL2_image

# Include files and parameters that are common to both client and server.
include( ../common/common.pri )

unix: LIBS += -lGL
win32: LIBS += -lopengl32

# Cliet headers
HEADERS += \
    ../../src/client/gui/viewport.hpp \
    ../../src/client/gui/view_frame.hpp \
    ../../src/client/gui/tools_menu/tools_menu.hpp \
    ../../src/client/gui/render_panel.hpp \
    ../../src/client/gui/main_window.hpp \
    ../../src/client/managers/scene/scene.hpp \
    ../../src/client/managers/como_app.hpp \
    ../../src/client/models/3d/meshes/mesh.hpp \
    ../../src/client/models/3d/camera.hpp \
    ../../src/client/models/utilities/msl/src/shader_loader.hpp \
    ../../src/client/gui/connection_wizard/connection_wizard.hpp \
    ../../src/client/gui/connection_wizard/connect_server_page.hpp \
    ../../src/client/gui/connection_wizard/create_server_page.hpp \
    ../../src/client/gui/connection_wizard/intro_page.hpp \
    ../../src/client/gui/connection_wizard/page_ids.hpp \
    ../../src/client/gui/users/users_list.hpp \
    ../../src/client/managers/server_interface/server_interface.hpp \
    ../../src/client/models/3d/lights/directional_light.hpp \
    ../../src/client/models/3d/lights/lights.hpp \
    ../../src/client/gui/tools_menu/creation_tab.hpp \
    ../../src/client/gui/tools_menu/transformation_tab.hpp \
    ../../src/client/gui/tools_menu/properties_tab/properties_tab.hpp \
    ../../src/client/gui/tools_menu/properties_tab/general_info_menu.hpp \
    ../../src/client/gui/tools_menu/properties_tab/mesh_info_menu.hpp \
    ../../src/client/models/3d/materials/material.hpp \
    ../../src/client/gui/utilities/color_button.hpp \
    ../../src/client/managers/managers/materials/materials_manager.hpp \
    ../../src/client/gui/materials/materials_editor.hpp \
    ../../src/client/managers/managers/materials/material_handler.hpp \
    ../../src/client/managers/managers/lights/lights_manager.hpp \
    ../../src/client/models/3d/lights/abstract_light.hpp \
    ../../src/client/models/utilities/open_gl.hpp \
    ../../src/client/gui/users/users_list_item.hpp \
    ../../src/client/managers/managers/primitives/client_primitives_manager.hpp \
    ../../src/client/gui/primitives/primitive_import_dialog.hpp \
    ../../src/client/gui/resources/resource_selector.hpp \
    ../../src/client/models/3d/textures/texture.hpp \
    ../../src/client/gui/rendering/viewport_tool_bar.hpp \
    ../../src/client/gui/menu_bar.hpp \
    ../../src/client/models/3d/meshes/abstract_mesh.hpp \
    ../../src/client/managers/selections/meshes/meshes_selection.hpp \
    ../../src/client/gui/rendering/viewport_view_menu.hpp \
    ../../src/client/gui/rendering/viewport_projection_menu.hpp \
    ../../src/client/managers/managers/resources/resources_manager.hpp \
    ../../src/client/managers/managers/meshes/meshes_manager.hpp \
    ../../src/client/managers/selections/lights/lights_selection.hpp \
    ../../src/client/managers/selections/resources/resources_selection.hpp \
    ../../src/client/managers/selections/resources/local_resources_selection.hpp \
    ../../src/client/managers/selections/lights/local_lights_selection.hpp \
    ../../src/client/managers/selections/meshes/local_meshes_selection.hpp \
    ../../src/client/gui/tools_menu/properties_tab/lights_menu.hpp \
    ../../src/client/models/3d/entity.hpp \
    ../../src/client/models/3d/drawable.hpp \
    ../../src/client/models/3d/entities_set.hpp \
    ../../src/client/models/3d/lights/light.hpp \
    ../../src/client/managers/managers/entities/entities_manager.hpp \
    ../../src/client/managers/managers/entities/specialized_entities_manager.hpp \
    ../../src/client/managers/selections/entities/entities_selection.hpp \
    ../../src/client/models/3d/abstract_entities_set.hpp \
    ../../src/client/managers/selections/entities/local_entities_selection.hpp \
    ../../src/client/managers/utilities/server_writer.hpp \
    ../../src/client/managers/managers/users/users_manager.hpp \
    ../../src/client/models/users/coloured_user.hpp \
    ../../src/client/managers/managers/resources/resources_ownership_requester.hpp \
    ../../src/client/managers/managers/resources/resource_commands_executer.hpp \
    ../../src/client/models/3d/sets/drawables_set.hpp \
    ../../src/client/models/3d/sets/pickables_set.hpp \
    ../../src/client/managers/managers/entities/abstract_entities_manager.hpp \
    ../../src/client/gui/materials/material_editor.hpp \
    ../../src/client/models/3d/meshes/system_mesh.hpp \
    ../../src/client/models/3d/meshes/imported_mesh.hpp \
    ../../src/client/gui/materials/materials_list_widget.hpp \
    ../../src/client/managers/managers/primitives/specialized_system_primitives_factory.hpp \
    ../../src/client/managers/managers/primitives/cubes_factory.hpp \
    ../../src/client/gui/tools_menu/system_primitives_creation_menu.hpp \
    ../../src/client/managers/managers/textures/textures_manager.hpp \
    ../../src/client/models/3d/meshes/triangles_group_with_texture_wall.hpp \
    ../../src/client/managers/managers/textures/texture_walls_manager.hpp \
    ../../src/client/gui/texture_walls/texture_walls_editor.hpp \
    ../../src/client/gui/utilities/resources_dropdown_list.hpp \
    ../../src/client/managers/managers/textures/texture_wall_handler.hpp \
    ../../src/client/gui/texture_walls/texture_wall_editor.hpp \
    ../../src/client/gui/textures/textures_viewer.hpp \
    ../../src/client/gui/textures/textures_gallery.hpp \
    ../../src/client/gui/textures/textures_gallery_item.hpp \
    ../../src/client/managers/managers/primitives/cones_factory.hpp \
    ../../src/client/managers/managers/primitives/cylinders_factory.hpp \
    ../../src/client/managers/managers/primitives/spheres_factory.hpp \
    ../../src/client/managers/managers/cameras/cameras_manager.hpp \
    ../../src/client/managers/selections/cameras/cameras_selection.hpp \
    ../../src/client/managers/selections/cameras/local_cameras_selection.hpp \
    ../../src/client/models/3d/auxiliar_lines_renderer.hpp \
    ../../src/client/gui/application.hpp \
    ../../src/client/managers/managers/primitives/system_primitives_factory.hpp \
    ../../src/client/models/3d/materials/abstract_material.hpp


# Client sources
SOURCES += \
    ../../src/client/gui/viewport.cpp \
    ../../src/client/gui/view_frame.cpp \
    ../../src/client/gui/tools_menu/tools_menu.cpp \
    ../../src/client/gui/render_panel.cpp \
    ../../src/client/gui/main_window.cpp \
    ../../src/client/managers/scene/scene.cpp \
    ../../src/client/managers/como_app.cpp \
    ../../src/client/models/3d/meshes/mesh.cpp \
    ../../src/client/models/3d/camera.cpp \
    ../../src/client/models/utilities/msl/src/shader_loader.cpp \
    ../../src/client/main.cpp \
    ../../src/client/gui/connection_wizard/connection_wizard.cpp \
    ../../src/client/gui/connection_wizard/connect_server_page.cpp \
    ../../src/client/gui/connection_wizard/create_server_page.cpp \
    ../../src/client/gui/connection_wizard/intro_page.cpp \
    ../../src/client/gui/users/users_list.cpp \
    ../../src/client/managers/server_interface/server_interface.cpp \
    ../../src/client/models/3d/lights/directional_light.cpp \
    ../../src/client/gui/tools_menu/creation_tab.cpp \
    ../../src/client/gui/tools_menu/transformation_tab.cpp \
    ../../src/client/gui/tools_menu/properties_tab/properties_tab.cpp \
    ../../src/client/gui/tools_menu/properties_tab/general_info_menu.cpp \
    ../../src/client/gui/tools_menu/properties_tab/mesh_info_menu.cpp \
    ../../src/client/models/3d/materials/material.cpp \
    ../../src/client/gui/utilities/color_button.cpp \
    ../../src/client/managers/managers/materials/materials_manager.cpp \
    ../../src/client/gui/materials/materials_editor.cpp \
    ../../src/client/managers/managers/materials/material_handler.cpp \
    ../../src/client/managers/managers/lights/lights_manager.cpp \
    ../../src/client/models/utilities/open_gl.cpp \
    ../../src/client/gui/users/users_list_item.cpp \
    ../../src/client/managers/managers/primitives/client_primitives_manager.cpp \
    ../../src/client/gui/primitives/primitive_import_dialog.cpp \
    ../../src/client/gui/resources/resource_selector.cpp \
    ../../src/client/models/3d/textures/texture.cpp \
    ../../src/client/gui/rendering/viewport_tool_bar.cpp \
    ../../src/client/gui/menu_bar.cpp \
    ../../src/client/managers/selections/meshes/meshes_selection.cpp \
    ../../src/client/gui/rendering/viewport_view_menu.cpp \
    ../../src/client/gui/rendering/viewport_projection_menu.cpp \
    ../../src/client/managers/managers/meshes/meshes_manager.cpp \
    ../../src/client/managers/selections/lights/lights_selection.cpp \
    ../../src/client/managers/selections/lights/local_lights_selection.cpp \
    ../../src/client/managers/selections/meshes/local_meshes_selection.cpp \
    ../../src/client/gui/tools_menu/properties_tab/lights_menu.cpp \
    ../../src/client/models/3d/entity.cpp \
    ../../src/client/models/3d/lights/light.cpp \
    ../../src/client/managers/managers/entities/entities_manager.cpp \
    ../../src/client/managers/selections/entities/entities_selection.cpp \
    ../../src/client/managers/selections/entities/local_entities_selection.cpp \
    ../../src/client/managers/utilities/server_writer.cpp \
    ../../src/client/managers/managers/users/users_manager.cpp \
    ../../src/client/models/users/coloured_user.cpp \
    ../../src/client/managers/managers/resources/resources_ownership_requester.cpp \
    ../../src/client/managers/managers/resources/resource_commands_executer.cpp \
    ../../src/client/models/3d/sets/drawables_set.cpp \
    ../../src/client/managers/managers/entities/abstract_entities_manager.cpp \
    ../../src/client/gui/materials/material_editor.cpp \
    ../../src/client/models/3d/meshes/system_mesh.cpp \
    ../../src/client/models/3d/meshes/imported_mesh.cpp \
    ../../src/client/gui/materials/materials_list_widget.cpp \
    ../../src/client/managers/managers/primitives/specialized_system_primitives_factory.cpp \
    ../../src/client/managers/managers/primitives/cubes_factory.cpp \
    ../../src/client/gui/tools_menu/system_primitives_creation_menu.cpp \
    ../../src/client/managers/managers/textures/textures_manager.cpp \
    ../../src/client/models/3d/meshes/triangles_group_with_texture_wall.cpp \
    ../../src/client/managers/managers/textures/texture_walls_manager.cpp \
    ../../src/client/gui/texture_walls/texture_walls_editor.cpp \
    ../../src/client/gui/utilities/resources_dropdown_list.cpp \
    ../../src/client/managers/managers/textures/texture_wall_handler.cpp \
    ../../src/client/gui/texture_walls/texture_wall_editor.cpp \
    ../../src/client/gui/textures/textures_viewer.cpp \
    ../../src/client/gui/textures/textures_gallery.cpp \
    ../../src/client/gui/textures/textures_gallery_item.cpp \
    ../../src/client/managers/managers/primitives/cones_factory.cpp \
    ../../src/client/managers/managers/primitives/cylinders_factory.cpp \
    ../../src/client/managers/managers/primitives/spheres_factory.cpp \
    ../../src/client/managers/managers/cameras/cameras_manager.cpp \
    ../../src/client/managers/selections/cameras/cameras_selection.cpp \
    ../../src/client/managers/selections/cameras/local_cameras_selection.cpp \
    ../../src/client/models/3d/auxiliar_lines_renderer.cpp \
    ../../src/client/gui/application.cpp \
    ../../src/client/managers/managers/primitives/system_primitives_factory.cpp
