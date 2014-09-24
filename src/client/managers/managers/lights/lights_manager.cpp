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

#include "lights_manager.hpp"

namespace como {

/***
 * 1. Construction
 ***/

LightsManager::LightsManager( ServerInterfacePtr server, LogPtr log, OpenGL* openGL ) :
    ResourceCommandsExecuter( server ),
    SpecializedEntitiesManager( server, log ),
    openGL_( openGL )
{}


/***
 * 3. Getters
 ***/

std::string LightsManager::getResourceName( const ResourceID& lightID ) const
{
    (void)( lightID );
    return "Light";
}


/***
 * 4. Lights management
 ***/

void LightsManager::requestDirectionalLightCreation()
{
    // Create a default light color.
    PackableColor lightColor( 255, 255, 255, 255 );

    sendCommandToServer(
                CommandConstPtr(
                    new DirectionalLightCreationCommand( localUserID(),
                                                         reserveResourceIDs( 1 ),
                                                         lightColor ) ) );
}


void LightsManager::addDirectionalLight( const ResourceID& lightID, const PackableColor& lightColor )
{
    std::unique_ptr< DirectionalLight >
            light( new DirectionalLight( lightColor, glm::vec3( 0.0f, -1.0f, 0.0f ), *openGL_ ) );

    getResourcesSelection( lightID.getCreatorID() )->addResource( lightID, std::move( light ) );

    //log()->debug( "\n\nDirectional light created: ", lightID, "\n\n" );

    notifyObservers();
}


/***
 * 5. Remote command execution
 ***/

// TODO: Change this and use a LightCommandConstPtr
void LightsManager::executeRemoteCommand( const LightCommand& command )
{
    switch( command.getType() ){
        case LightCommandType::LIGHT_CREATION:{
            const LightCreationCommand& lightCreationCommand =
                    dynamic_cast< const LightCreationCommand& >( command );

            switch( lightCreationCommand.getLightType() ){
                case LightType::DIRECTIONAL_LIGHT:{
                    const DirectionalLightCreationCommand& directionalLightCreationCommand =
                            dynamic_cast< const DirectionalLightCreationCommand& >( command );

                    addDirectionalLight( directionalLightCreationCommand.getResourceID(),
                                         directionalLightCreationCommand.getLightColor()
                                         );
                }break;
            }
        }break;
        case LightCommandType::LIGHT_COLOR_CHANGE:{
            const LightColorChangeCommand& lightCommand =
                    dynamic_cast< const LightColorChangeCommand& >( command );

            getResourcesSelection( lightCommand.getUserID() )->setLightColor( lightCommand.getLightColor() );

            notifyObservers();
        }break;
        case LightCommandType::LIGHT_AMBIENT_COEFFICIENT_CHANGE:{
            const LightAmbientCoefficientChangeCommand& lightCommand =
                    dynamic_cast< const LightAmbientCoefficientChangeCommand& >( command );

            getResourcesSelection( lightCommand.getUserID() )->setAmbientCoefficient( lightCommand.getAmbientCoefficient() );

            notifyObservers();
        }break;
        case LightCommandType::LIGHT_CREATION_RESPONSE:{
            const LightCreationResponseCommand& lightCommand =
                    dynamic_cast< const LightCreationResponseCommand& >( command );

            if( lightCommand.getResponse() ){
                addDirectionalLight( lightCommand.getResourceID(),
                                     PackableColor( 255, 255, 255, 255 ) ); // TODO: Use same color as in request or remove such color from request and use WHITE always.
            }else{
                // TODO: Move this to a GUI class.
                QMessageBox errorMsg( QMessageBox::Critical,
                                      "Could't create a directional light",
                                      "Remote attempt to create a directional light with no room for it!" );
                errorMsg.exec();
            }
        }break;
    }
}


/***
 * 6. Shader communication
 ***/

void LightsManager::sendLightsToShader( OpenGL &openGL, const glm::mat4& viewMatrix ) const
{
    openGL.setShadingMode( ShadingMode::SOLID_LIGHTING );
    for( const auto& lightSelection : resourcesSelections_ ){
        lightSelection.second->sendToShader( openGL, viewMatrix );
    }
}


/***
 * 8. Updating
 ***/

void LightsManager::update()
{
    notifyObservers();
}

} // namespace como
