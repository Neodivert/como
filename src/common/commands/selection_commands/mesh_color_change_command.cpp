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

#include "mesh_color_change_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

MeshColorChangeCommand::MeshColorChangeCommand() :
    SelectionCommand( SelectionCommandType::MESH_COLOR_CHANGE, 0 )
{
    addPackable( &meshColor_ );
}


MeshColorChangeCommand::MeshColorChangeCommand( UserID userID, const std::uint8_t* color ) :
    SelectionCommand( SelectionCommandType::MESH_COLOR_CHANGE, userID ),
    meshColor_( color )
{
    addPackable( &meshColor_ );
}


/***
 * 3. Getters
 ***/

const std::uint8_t* MeshColorChangeCommand::getMeshColor() const
{
    return &( ( meshColor_.getValues() )[0] );
}

} // namespace como
