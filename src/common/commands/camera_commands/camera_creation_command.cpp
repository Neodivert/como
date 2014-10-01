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

#include "camera_creation_command.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace como {


/***
 * 1. Construction
 ***/

CameraCreationCommand::CameraCreationCommand() :
    CameraCommand( CameraCommandType::CAMERA_CREATION, NO_RESOURCE, NO_USER )
{
    addPackable( &cameraEye_ );
    addPackable( &cameraCenter_ );
    addPackable( &cameraUp_ );
}


CameraCreationCommand::CameraCreationCommand( ResourceID cameraID,
                                              const glm::vec3 cameraCenter,
                                              const glm::vec3 cameraEye,
                                              const glm::vec3 cameraUp ) :
    CameraCommand( CameraCommandType::CAMERA_CREATION, cameraID, cameraID.getCreatorID() ),
    cameraEye_( glm::value_ptr( cameraEye ) ),
    cameraCenter_( glm::value_ptr( cameraCenter ) ),
    cameraUp_( glm::value_ptr( cameraUp ) )
{
    addPackable( &cameraEye_ );
    addPackable( &cameraCenter_ );
    addPackable( &cameraUp_ );
}


CameraCreationCommand::CameraCreationCommand( const CameraCreationCommand &b ) :
    CameraCommand( b ),
    cameraEye_( b.cameraEye_ ),
    cameraCenter_( b.cameraCenter_ ),
    cameraUp_( b.cameraUp_ )
{
    addPackable( &cameraEye_ );
    addPackable( &cameraCenter_ );
    addPackable( &cameraUp_ );
}


/***
 * 3. Getters
 ***/

glm::vec3 CameraCreationCommand::cameraEye() const
{
    return glm::vec3(
                cameraEye_.getValues()[0],
            cameraEye_.getValues()[1],
            cameraEye_.getValues()[2] );
}


glm::vec3 CameraCreationCommand::cameraCenter() const
{
    return glm::vec3(
                cameraCenter_.getValues()[0],
            cameraCenter_.getValues()[1],
            cameraCenter_.getValues()[2] );
}


glm::vec3 CameraCreationCommand::cameraUp() const
{
    return glm::vec3(
                cameraUp_.getValues()[0],
            cameraUp_.getValues()[1],
            cameraUp_.getValues()[2] );
}



} // namespace como
