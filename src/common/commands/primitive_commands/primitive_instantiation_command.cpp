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

#include "primitive_instantiation_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

PrimitiveInstantiationCommand::PrimitiveInstantiationCommand() :
    PrimitiveCommand( PrimitiveCommandType::PRIMITIVE_INSTANTIATION, 0, ResourceID() ), // TODO: Remove ResourceID().
    primitiveID_( NO_RESOURCE ),
    meshID_( NO_RESOURCE ),
    materialID_( NO_RESOURCE )
{
    addPackable( &primitiveID_ );
    addPackable( &meshID_ );
    addPackable( &materialID_ );
}


PrimitiveInstantiationCommand::PrimitiveInstantiationCommand( ResourceID primitiveID, PackableDrawableID drawableID, const MaterialID& materialID ) :
    PrimitiveCommand( PrimitiveCommandType::PRIMITIVE_INSTANTIATION, primitiveID.getCreatorID(), primitiveID ),
    primitiveID_( primitiveID ),
    meshID_( ResourceID( drawableID.creatorID.getValue(), static_cast< ResourceIndex >( drawableID.drawableIndex.getValue() ) ) ),
    materialID_( ResourceID( materialID.getCreatorID(), static_cast< ResourceIndex >( materialID.getMaterialIndex() ) ) )
{
    addPackable( &primitiveID_ );
    addPackable( &meshID_ );
    addPackable( &materialID_ );
}


PrimitiveInstantiationCommand::PrimitiveInstantiationCommand( const PrimitiveInstantiationCommand& b ) :
    PrimitiveCommand( b ),
    primitiveID_( b.primitiveID_ ),
    meshID_( b.meshID_ ),
    materialID_( b.materialID_ )
{
    addPackable( &primitiveID_ );
    addPackable( &meshID_ );
    addPackable( &materialID_ );
}


/***
 * 3. Getters
 ***/

ResourceID PrimitiveInstantiationCommand::getPrimitiveID() const
{
    return primitiveID_.getValue();
}


ResourceID PrimitiveInstantiationCommand::getMeshID() const
{
    return meshID_.getValue();
}


ResourceID PrimitiveInstantiationCommand::getMaterialID() const
{
    return materialID_.getValue();
}

} // namespace como
