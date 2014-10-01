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
    activeCameraID_( NO_RESOURCE )
{}


/***
 * 3. Getters
 ***/

glm::mat4 CamerasManager::activeCameraViewMatrix() const
{
    return activeCameraSelection().cameraViewMatrix( activeCameraID_ );
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
 * 5. Shader communication
 ***/

void CamerasManager::sendActiveCameraToShader() const
{
    activeCameraSelection().sendCameraToShader( *openGL_, activeCameraID_ );
}


/***
 * 7. Remote command creation
 ***/

void CamerasManager::createCamera( const ResourceID &cameraID,
                                   const glm::vec3 &cameraCenter,
                                   const glm::vec3 &cameraEye,
                                   const glm::vec3 &cameraUp )
{
    if( activeCameraID_ == NO_RESOURCE ){
        activeCameraID_ = cameraID;
    }

    std::unique_ptr< Camera > camera( new Camera( *openGL_,
                                                  cameraCenter,
                                                  cameraEye,
                                                  cameraUp ) );

    resourcesSelections_.at( cameraID.getCreatorID() )->addResource( cameraID, std::move( camera ) );
}




/****
 * 8. Private getters
 ***/

CamerasSelection &CamerasManager::activeCameraSelection() const
{
    // Search the selection containing the active camera and return it.
    for( const auto& camerasSelection : resourcesSelections_ ){
        if( camerasSelection.second->containsResource( activeCameraID_ ) ){
            return *( camerasSelection.second );
        }
    }

    throw std::runtime_error( "Active camera not found" );
}

} // namespace como
