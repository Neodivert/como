/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
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
#include "user_commands/parameter_change_command.hpp"

// Drawable commands
#include "drawable_commands/cube_creation_command.hpp"
#include "drawable_commands/drawable_selection_command.hpp"

// Selection commands
#include "selection_commands/full_deselection_command.hpp"
#include "selection_commands/selection_response_command.hpp"
#include "selection_commands/selection_transformation_command.hpp"
#include "selection_commands/selection_deletion_command.hpp"

// Primitive commands
#include "primitive_commands/primitive_creation_command.hpp"

#endif // COMMANDS_HPP
