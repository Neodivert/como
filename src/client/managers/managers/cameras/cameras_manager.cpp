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

#include "cameras_manager.hpp"

namespace como {

/***
 * 1. Construction
 ***/

CamerasManager::CamerasManager( OpenGL& openGL, ServerInterfacePtr server, LogPtr log) :
    ResourceCommandsExecuter( server ),
    SpecializedEntitiesManager( server, log ),
    openGL_( &openGL ),
    activeCamera_( nullptr )
{}


/***
 * 3. Getters
 ***/

const Camera &CamerasManager::activeCamera() const
{
    return *activeCamera_;
}


/***
 * 4. Commands execution
 ***/

void CamerasManager::executeRemoteCommand( const CameraCommand &command )
{
    switch( command.getType() ){
        case CameraCommandType::CAMERA_CREATION:{
            const CameraCreationCommand& creationCommand =
                    dynamic_cast< const CameraCreationCommand& >( command );

            createCamera( creationCommand.cameraID(),
                          creationCommand.cameraCenter(),
                          creationCommand.cameraEye(),
                          creationCommand.cameraUp() );
        }break;
    }
}


/***
 * 6. Resources management
 ***/

void CamerasManager::clearResourcesSelection( UserID currentOwner )
{
    unlockResourcesSelection( currentOwner );
}


/***
 * 7. Remote command creation
 ***/

void CamerasManager::createCamera( const ResourceID &cameraID,
                                   const glm::vec3 &cameraCenter,
                                   const glm::vec3 &cameraEye,
                                   const glm::vec3 &cameraUp )
{


    std::unique_ptr< Camera > camera( new Camera( *openGL_,
                                                  cameraCenter,
                                                  cameraEye,
                                                  cameraUp ) );

    if( activeCamera_ == nullptr ){
        activeCamera_ = camera.get();
    }

    resourcesSelections_.at( cameraID.getCreatorID() )->addResource( cameraID, std::move( camera ) );
}

} // namespace como
