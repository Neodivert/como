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

LightsManager::LightsManager( ServerInterfacePtr server, LogPtr log ) :
    AbstractEntitiesManager( server, log ),
    currentLight_( nullptr )
{
    GLint i=0;

    // TODO: Remove the "3" and retrieve it from shader
    // (MAX_LIGHTS - 1).
    for( i=3; i>=0; i-- ){
        freeLightIndices_.push( i );
    }

    // TODO: Remove the "3" and retrieve it from shader
    // (MAX_DIRECTIONAL_LIGHTS - 1).
    for( i=3; i>=0; i-- ){
        freeDirectionalLightIndices_.push( i );
    }
}


/***
 * 3. Getters
 ***/

LightHandlerPtr LightsManager::getCurrentLight() const
{
    return currentLight_;
}


std::string LightsManager::getResourceName( const ResourceID& lightID ) const
{
    (void)( lightID );
    return "Light";
}


/***
 * 4. Lights management
 ***/

bool LightsManager::createDirectionalLight()
{
    // FIXME: Duplicated code (except for error message).
    GLint lightIndex;
    GLuint directionalLightIndex;

    // TODO: What happens if two users create the last allowed directional
    // light at the same time? I need confirmation from server...
    if( !freeDirectionalLightIndices_.size() ){
        return false;
    }

    // Reserve both directional and light indices for the new light.
    lightIndex = freeLightIndices_.top();
    directionalLightIndex = freeDirectionalLightIndices_.top();
    freeLightIndices_.pop();
    freeDirectionalLightIndices_.pop();

    // Create a default material for the light.
    MaterialConstPtr lightMaterial( new Material( PackableColor( 255, 0, 0, 255 ) ) );

    // Create a default light color.
    PackableColor lightColor( 255, 0, 0, 255 );

    // Create a struct with the light's properties.
    DirectionalLightPtr light(
                new DirectionalLight( directionalLightIndex, lightIndex, lightColor, glm::vec3( 0.0f, -1.0f, 0.0f ), lightMaterial ) );

    // Add the created light to the Drawables Manager and retrieve the ID given
    // to it.
    ResourceID lightID = getLocalResourcesSelection()->addResource( light );

    sendCommandToServer( CommandConstPtr( new DirectionalLightCreationCommand( localUserID(), lightID, lightColor ) ) );

    //log()->debug( "\n\nDirectional light created: ", lightID, "\n\n" );

    notifyObservers();

    return true;
}


void LightsManager::addDirectionalLight( const ResourceID& lightID, const PackableColor& lightColor )
{
    // FIXME: Duplicated code (except for error message).
    GLint lightIndex;
    GLuint directionalLightIndex;

    if( !freeDirectionalLightIndices_.size() ){
        QMessageBox errorMsg( QMessageBox::Critical,
                              "Could't create a directional light",
                              "Remote attempt to create a directional light with no room for it!" );
        errorMsg.exec();
        return;
    }

    // Reserve both directional and light indices for the new light.
    lightIndex = freeLightIndices_.top();
    directionalLightIndex = freeDirectionalLightIndices_.top();
    freeLightIndices_.pop();
    freeDirectionalLightIndices_.pop();

    MaterialConstPtr lightMaterial( new Material( PackableColor( 255, 0, 0, 255 ) ) );

    DirectionalLightPtr light( new DirectionalLight( directionalLightIndex, lightIndex, lightColor, glm::vec3( 0.0f, -1.0f, 0.0f ), lightMaterial ) );

    getResourcesSelection( lightID.getCreatorID() )->addResource( lightID, light );


    //log()->debug( "\n\nDirectional light created: ", lightID, "\n\n" );

    notifyObservers();
}


void LightsManager::selectLight( const ResourceID lightID )
{
    (void)( lightID );
    /*
    if( currentLight_ ){
        currentLight_->removeObserver( this );
    }

    currentLight_ = LightHandlerPtr( new LightHandler( lights_.at( lightID ), lightID, server() ) );

    currentLight_->Observable::addObserver( this );

    notifyObservers();
    */
}


void LightsManager::removeLight( ResourceID lightID )
{
    (void)( lightID );
    /*
    log()->debug( "LightsManager - removing ID ", lightID, "\n" );

    // Retrieve the light to be removed.
    LightSharedPtr light = lights_.at( lightID );

    // Free the indices held by the light.
    freeLightIndices_.push( light->getBaseLightIndex() );
    switch( light->getLightType() ){
        case LightType::DIRECTIONAL_LIGHT:
            freeDirectionalLightIndices_.push( light->getLightIndex() );
        break;
    }

    // Remove the light from lights_ vector and signal it.
    lights_.erase( lightID );

    notifyObservers();
    */
}


/***
 * 5. Remote command execution
 ***/

// TODO: Change this and use a LightCommandConstPtr
void LightsManager::executeRemoteCommand( LightCommandConstPtr command )
{
    switch( command->getType() ){
        case LightCommandType::LIGHT_CREATION:{
            const LightCreationCommand* lightCreationCommand =
                    dynamic_cast< const LightCreationCommand* >( command.get() );

            switch( lightCreationCommand->getLightType() ){
                case LightType::DIRECTIONAL_LIGHT:{
                    const DirectionalLightCreationCommand* directionalLightCreationCommand =
                            dynamic_cast< const DirectionalLightCreationCommand* >( command.get() );

                    addDirectionalLight( directionalLightCreationCommand->getResourceID(),
                                         directionalLightCreationCommand->getLightColor()
                                         );
                }break;
            }
        }break;
        case LightCommandType::LIGHT_COLOR_CHANGE:{
            const LightColorChangeCommand* lightCommand =
                    dynamic_cast< const LightColorChangeCommand* >( command.get() );

            getResourcesSelection( lightCommand->getUserID() )->setLightColor( lightCommand->getLightColor() );

            notifyObservers();
        }break;
        case LightCommandType::LIGHT_AMBIENT_COEFFICIENT_CHANGE:{
            const LightAmbientCoefficientChangeCommand* lightCommand =
                    dynamic_cast< const LightAmbientCoefficientChangeCommand* >( command.get() );

            getResourcesSelection( lightCommand->getUserID() )->setAmbientCoefficient( lightCommand->getAmbientCoefficient() );

            notifyObservers();
        }break;
    }
}


/***
 * 8. Updating
 ***/

void LightsManager::update()
{
    notifyObservers();
}


/***
 * 9. Auxiliar methods
 ***/

unsigned int LightsManager::getNextFreeLightIndex( LightType lightType )
{
    switch( lightType ){
        case LightType::DIRECTIONAL_LIGHT:
        break;
    }

    Q_UNUSED( lightType )

    /*
    // TODO: Retrieve this value from shader depending on the value of
    // lightType.
    Q_UNUSED( lightType )
    const unsigned int MAX_REQUESTED_LIGHTS = 4;
    */

    return 0;
}

void LightsManager::highlightLight( ResourceID lightID )
{
    (void)( lightID );
    //drawablesManager_->highlightProperty( lights_.at( lightID ).get() );
}


void LightsManager::removeHighlights()
{
    //drawablesManager_->highlightProperty( nullptr );
}

} // namespace como
