/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published by
 * the Free Software Foundation.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

// User commands
#include "user_commands/user_connection_command.hpp"
#include "user_commands/user_disconnection_command.hpp"
#include "user_commands/user_parameter_change_command.hpp"

// Selection commands
#include "selection_commands/selection_transformation_command.hpp"

// Primitive commands
#include "primitive_commands/primitive_commands.hpp"

// Primitive category commands
#include "primitive_category_commands/primitive_category_commands.hpp"

// Material commands
#include "material_commands/material_commands.hpp" // TODO: Make this with the rest of commands.

// MeshLight commands
#include "light_commands/light_commands.hpp"

// Resource commands
#include "resource_commands/resource_commands.hpp"

// Resources selection commands
#include "resources_selection_commands/resources_selection_command.hpp"

// Geometric primitives commands
#include "geometric_primitive_command/geometric_primitive_commands.hpp"

// Texture commands
#include "texture_commands/texture_commands.hpp"

// Texture wall commands
#include "texture_wall_commands/texture_wall_commands.hpp"

// Camera commands
#include "camera_commands/camera_commands.hpp"

#endif // COMMANDS_HPP
