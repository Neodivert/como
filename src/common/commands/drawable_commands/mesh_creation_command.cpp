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

#include "mesh_creation_command.hpp"

namespace como {


/***
 * 1. Construction
 ***/

MeshCreationCommand::MeshCreationCommand() :
    DrawableCommand( DrawableCommandType::MESH_CREATION, 0, NULL_DRAWABLE_ID )
{
    addPackable( &primitiveID_ );
    addPackable( &color_ );
}


MeshCreationCommand::MeshCreationCommand( UserID userID, PackableDrawableID drawableID, PrimitiveID primitiveID, const std::uint8_t* color ) :
    DrawableCommand( DrawableCommandType::MESH_CREATION, userID, drawableID ),
    primitiveID_( primitiveID )
{
    setColor( color );

    addPackable( &primitiveID_ );
    addPackable( &color_ );
}


MeshCreationCommand::MeshCreationCommand( const MeshCreationCommand& b ) :
    DrawableCommand( b ),
    color_( b.color_ )
{
    addPackable( &primitiveID_ );
    addPackable( &color_ );
}


/***
 * 3. Getters
 ***/

PrimitiveID MeshCreationCommand::getPrimitiveID() const
{
    return primitiveID_.getValue();
}


const std::uint8_t* MeshCreationCommand::getColor() const
{
    return color_.getValue();
}


/***
 * 4. Setters
 ***/

void MeshCreationCommand::setColor( const std::uint8_t* color )
{
    color_ = color;
}

} // namespace como
