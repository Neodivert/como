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

#include "mesh_creation_command.hpp"

namespace como {


/***
 * 1. Construction
 ***/

MeshCreationCommand::MeshCreationCommand( MeshType meshType, PackableDrawableID drawableID, const MaterialID& materialID ) :
    DrawableCommand( DrawableCommandType::MESH_CREATION, drawableID.creatorID.getValue(), drawableID ),
    meshType_( meshType ),
    materialID_( materialID )
{
    addPackable( &meshType_ );
    addPackable( &materialID_ );
}


MeshCreationCommand::MeshCreationCommand( const MeshCreationCommand& b ) :
    DrawableCommand( b ),
    meshType_( b.meshType_ ),
    materialID_( b.materialID_ )
{
    addPackable( &meshType_ );
    addPackable( &materialID_ );
}


/***
 * 3. Getters
 ***/

MeshType MeshCreationCommand::getMeshType() const
{
    return meshType_.getValue();
}


MaterialID MeshCreationCommand::getMaterialID() const
{
    return materialID_.getValue();
}


/***
 * 4. Setters
 ***/

void MeshCreationCommand::setMaterialID( const MaterialID& id )
{
    materialID_ = id;
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
