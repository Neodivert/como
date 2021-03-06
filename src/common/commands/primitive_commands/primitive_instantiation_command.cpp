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
    materialID_( NO_RESOURCE ),
    centroid_( 0.0f, 0.0f, 0.0f )
{
    addPackable( &primitiveID_ );
    addPackable( &meshID_ );
    addPackable( &materialID_ );
    addPackable( &centroid_ );
}


PrimitiveInstantiationCommand::PrimitiveInstantiationCommand( UserID userID, ResourceID primitiveID, ResourceID drawableID, const ResourceID& materialID, const glm::vec3& centroid ) :
    PrimitiveCommand( PrimitiveCommandType::PRIMITIVE_INSTANTIATION, userID, primitiveID ),
    primitiveID_( primitiveID ),
    meshID_( drawableID ),
    materialID_( materialID ),
    centroid_( centroid.x, centroid.y, centroid.z )
{
    addPackable( &primitiveID_ );
    addPackable( &meshID_ );
    addPackable( &materialID_ );
    addPackable( &centroid_ );
}


PrimitiveInstantiationCommand::PrimitiveInstantiationCommand( const PrimitiveInstantiationCommand& b ) :
    PrimitiveCommand( b ),
    primitiveID_( b.primitiveID_ ),
    meshID_( b.meshID_ ),
    materialID_( b.materialID_ ),
    centroid_( b.centroid_ )
{
    addPackable( &primitiveID_ );
    addPackable( &meshID_ );
    addPackable( &materialID_ );
    addPackable( &centroid_ );
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


glm::vec3 PrimitiveInstantiationCommand::centroid() const
{
    return glm::vec3(
                centroid_.getValues()[0],
                centroid_.getValues()[1],
                centroid_.getValues()[2]
                );
}

} // namespace como
