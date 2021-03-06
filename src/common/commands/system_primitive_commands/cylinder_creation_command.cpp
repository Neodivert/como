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

#include "cylinder_creation_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

CylinderCreationCommand::CylinderCreationCommand() :
    SystemPrimitiveCommand( SystemPrimitiveCommandType::CYLINDER_CREATION, NO_RESOURCE, NO_RESOURCE, NO_RESOURCE, 3 ),
    cylinderHeight_( 1.0f ),
    cylinderRadius_( 1.0f ),
    cylinderNRadialVertices_( 16 )
{
    addPackable( &cylinderHeight_ );
    addPackable( &cylinderRadius_ );
    addPackable( &cylinderNRadialVertices_ );
}


CylinderCreationCommand::CylinderCreationCommand( const ResourceID &cylinderID,
                                          const ResourceID &materialID,
                                          const ResourceID &firstTextureWallID,
                                          float coneHeight,
                                          float coneRadius,
                                          unsigned int coneNBaseVertices,
                                          const glm::vec3& centroid ) :
    SystemPrimitiveCommand( SystemPrimitiveCommandType::CYLINDER_CREATION, cylinderID, materialID, firstTextureWallID, 3, centroid ),
    cylinderHeight_( coneHeight ),
    cylinderRadius_( coneRadius ),
    cylinderNRadialVertices_( coneNBaseVertices )
{
    addPackable( &cylinderHeight_ );
    addPackable( &cylinderRadius_ );
    addPackable( &cylinderNRadialVertices_ );
}


CylinderCreationCommand::CylinderCreationCommand(const CylinderCreationCommand &b ) :
    SystemPrimitiveCommand( b ),
    cylinderHeight_( b.cylinderHeight_ ),
    cylinderRadius_( b.cylinderRadius_ ),
    cylinderNRadialVertices_( b.cylinderNRadialVertices_ )
{
    addPackable( &cylinderHeight_ );
    addPackable( &cylinderRadius_ );
    addPackable( &cylinderNRadialVertices_ );
}


/***
 * 3. Getters
 ***/

float CylinderCreationCommand::cylinderHeight() const
{
    return cylinderHeight_.getValue();
}


float CylinderCreationCommand::cylinderRadius() const
{
    return cylinderRadius_.getValue();
}


std::uint16_t CylinderCreationCommand::cylinderNRadialVertices() const
{
    return cylinderNRadialVertices_.getValue();
}

} // namespace como
