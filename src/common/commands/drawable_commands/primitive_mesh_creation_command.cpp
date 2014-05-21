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

#include "primitive_mesh_creation_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

PrimitiveMeshCreationCommand::PrimitiveMeshCreationCommand() :
    MeshCreationCommand( MeshType::PRIMITIVE_MESH, NULL_DRAWABLE_ID, PackableColor() ), // TODO: Remove PackableColor()
    primitiveID_( 0 )
{
    addPackable( &primitiveID_ );
}


PrimitiveMeshCreationCommand::PrimitiveMeshCreationCommand( PackableDrawableID drawableID, PrimitiveID primitiveID, const PackableColor& meshColor ) :
    MeshCreationCommand( MeshType::PRIMITIVE_MESH, drawableID, meshColor ),
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
