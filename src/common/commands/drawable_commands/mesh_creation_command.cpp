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

MeshCreationCommand::MeshCreationCommand( MeshType meshType, PackableDrawableID drawableID, const std::uint8_t* color ) :
    DrawableCommand( DrawableCommandType::MESH_CREATION, drawableID.creatorID.getValue(), drawableID ),
    meshType_( meshType ),
    meshColor_( color )
{
    addPackable( &meshType_ );
    addPackable( &meshColor_ );
}


MeshCreationCommand::MeshCreationCommand( const MeshCreationCommand& b ) :
    DrawableCommand( b ),
    meshType_( b.meshType_ ),
    meshColor_( b.meshColor_ )
{
    addPackable( &meshType_ );
    addPackable( &meshColor_ );
}


/***
 * 3. Getters
 ***/

MeshType MeshCreationCommand::getMeshType() const
{
    return meshType_.getValue();
}


const std::uint8_t* MeshCreationCommand::getMeshColor() const
{
    return &( ( meshColor_.getValues() )[0] );
}


/***
 * 4. Setters
 ***/

void MeshCreationCommand::setMeshColor( const std::uint8_t* color )
{
    meshColor_ = color;
}


/***
 * 5. Buffer pre-reading
 ***/

MeshType MeshCreationCommand::getMeshType( const void* buffer )
{
    DrawableCommand drawableCommand( DrawableCommandType::MESH_CREATION, NO_USER, NULL_DRAWABLE_ID );

    // FIXME: Compute packable's offset without creating an extra auxiliar
    // command.
    return static_cast< MeshType >( static_cast< const std::uint8_t *>( buffer )[drawableCommand.getPacketSize()] );
}

} // namespace como
