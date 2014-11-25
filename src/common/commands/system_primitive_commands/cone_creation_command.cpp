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

#include "cone_creation_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

ConeCreationCommand::ConeCreationCommand() :
    SystemPrimitiveCommand( SystemPrimitiveCommandType::CONE_CREATION, NO_RESOURCE, NO_RESOURCE, NO_RESOURCE, 2 ),
    coneHeight_( 1.0f ),
    coneRadius_( 1.0f ),
    coneNBaseVertices_( 16 )
{
    addPackable( &coneHeight_ );
    addPackable( &coneRadius_ );
    addPackable( &coneNBaseVertices_ );
}


ConeCreationCommand::ConeCreationCommand( const ResourceID &coneID,
                                          const ResourceID &materialID,
                                          const ResourceID &firstTextureWallID,
                                          float coneHeight,
                                          float coneRadius,
                                          unsigned int coneNBaseVertices,
                                          const glm::vec3& centroid ) :
    SystemPrimitiveCommand( SystemPrimitiveCommandType::CONE_CREATION, coneID, materialID, firstTextureWallID, 2, centroid ),
    coneHeight_( coneHeight ),
    coneRadius_( coneRadius ),
    coneNBaseVertices_( coneNBaseVertices )
{
    addPackable( &coneHeight_ );
    addPackable( &coneRadius_ );
    addPackable( &coneNBaseVertices_ );
}


ConeCreationCommand::ConeCreationCommand(const ConeCreationCommand &b ) :
    SystemPrimitiveCommand( b ),
    coneHeight_( b.coneHeight_ ),
    coneRadius_( b.coneRadius_ ),
    coneNBaseVertices_( b.coneNBaseVertices_ )
{
    addPackable( &coneHeight_ );
    addPackable( &coneRadius_ );
    addPackable( &coneNBaseVertices_ );
}


/***
 * 3. Getters
 ***/

float ConeCreationCommand::coneHeight() const
{
    return coneHeight_.getValue();
}


float ConeCreationCommand::coneRadius() const
{
    return coneRadius_.getValue();
}


std::uint16_t ConeCreationCommand::coneNBaseVertices() const
{
    return coneNBaseVertices_.getValue();
}

} // namespace como
