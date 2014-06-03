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

#include "primitive_mesh_creation_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

PrimitiveMeshCreationCommand::PrimitiveMeshCreationCommand() :
    MeshCreationCommand( MaterialMeshType::PRIMITIVE_MESH, NULL_DRAWABLE_ID, MaterialID() ), // TODO: Remove MaterialID()
    primitiveID_( 0 )
{
    addPackable( &primitiveID_ );
}


PrimitiveMeshCreationCommand::PrimitiveMeshCreationCommand( PackableDrawableID drawableID, PrimitiveID primitiveID, const MaterialID& materialID ) :
    MeshCreationCommand( MaterialMeshType::PRIMITIVE_MESH, drawableID, materialID ),
    primitiveID_( primitiveID )
{
    addPackable( &primitiveID_ );
}


PrimitiveMeshCreationCommand::PrimitiveMeshCreationCommand( const PrimitiveMeshCreationCommand& b ) :
    MeshCreationCommand( b ),
    primitiveID_( b.primitiveID_ )
{
    addPackable( &primitiveID_ );
}


/***
 * 3. Getters
 ***/

PrimitiveID PrimitiveMeshCreationCommand::getPrimitiveID() const
{
    return primitiveID_.getValue();
}

} // namespace como
