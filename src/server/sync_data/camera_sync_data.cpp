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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "camera_sync_data.hpp"

namespace como {

/***
 * 1. Construction
 ***/

CameraSyncData::CameraSyncData( const CameraCreationCommand &cameraCreationCommand ) :
    EntitySyncData( &cameraCreationCommand,
                    cameraCreationCommand.cameraID(),
                    glm::vec3( 0.0f ) ) // TODO: Retrieve real centroid
{
    setModelMatrix(
                glm::inverse(
                    glm::lookAt(
                        cameraCreationCommand.cameraEye(),
                        cameraCreationCommand.cameraCenter(),
                        cameraCreationCommand.cameraUp() ) ) );
}



} // namespace como
