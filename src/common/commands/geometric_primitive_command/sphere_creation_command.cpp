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

#include "sphere_creation_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

SphereCreationCommand::SphereCreationCommand() :
    GeometricPrimitiveCommand( GeometricPrimitiveCommandType::SPHERE_CREATION, NO_RESOURCE, NO_RESOURCE, NO_RESOURCE, 1 ),
    sphereRadius_( 1.0f ),
    sphereNDivisions_( 16 )
{
    addPackable( &sphereRadius_ );
    addPackable( &sphereNDivisions_ );
}


SphereCreationCommand::SphereCreationCommand( const ResourceID &sphereID, const ResourceID &materialID, const ResourceID &firstTextureWallID, float sphereRadius, std::uint16_t sphereNDivisions, const glm::vec3& centroid ) :
    GeometricPrimitiveCommand( GeometricPrimitiveCommandType::SPHERE_CREATION, sphereID, materialID, firstTextureWallID, 1, centroid ),
    sphereRadius_( sphereRadius ),
    sphereNDivisions_( sphereNDivisions )
{
    addPackable( &sphereRadius_ );
    addPackable( &sphereNDivisions_ );
}


SphereCreationCommand::SphereCreationCommand( const SphereCreationCommand &b ) :
    GeometricPrimitiveCommand( b ),
    sphereRadius_( b.sphereRadius_ ),
    sphereNDivisions_( b.sphereNDivisions_ )
{
    addPackable( &sphereRadius_ );
    addPackable( &sphereNDivisions_ );
}


/***
 * 3. Getters
 ***/

float SphereCreationCommand::sphereRadius() const
{
    return sphereRadius_.getValue();
}


std::uint16_t SphereCreationCommand::sphereNDivisions() const
{
    return sphereNDivisions_.getValue();
}


} // namespace como
