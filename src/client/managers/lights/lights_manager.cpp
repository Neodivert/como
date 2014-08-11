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

LightsManager::LightsManager( DrawablesManagerPtr drawablesManager, ServerInterfacePtr server, LogPtr log ) :
    ResourcesManager( server, log ),
    drawablesManager_( drawablesManager )
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


    drawablesManager_->addObserver( this );
}


/***
 * 3. Lights management
 ***/

void LightsManager::createDirectionalLight()
{
    // FIXME: Duplicated code (except for error message).
    GLint lightIndex;
    GLuint directionalLightIndex;

    if( !freeDirectionalLightIndices_.size() ){
        QMessageBox errorMsg( QMessageBox::Warning,
                              "Could't create a directional light",
                              "Reached the limit of allowed directional lights" );
        errorMsg.exec();
        return;
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
    DirectionalLightPropertiesSharedPtr lightProperties(
                new DirectionalLightProperties( directionalLightIndex, lightIndex, lightColor, glm::vec3( 0.0f, -1.0f, 0.0f ) ) );

    // Create a light with the previous light and material properties.
    DrawablePtr light = DrawablePtr( new DirectionalLight( lightProperties, lightMaterial ) );

    // Add the created light to the Drawables Manager and retrieve the ID given
    // to it.
    ResourceID lightID = drawablesManager_->addDrawable( light );

    // Insert the light's properties into the lights map.
    lights_.insert( std::pair< ResourceID, LightPropertiesSharedPtr >(
                        lightID,
                        lightProperties
                        ));

    sendCommandToServer( CommandConstPtr( new DirectionalLightCreationCommand( localUserID(), lightID, lightColor ) ) );

    log()->debug( "\n\nDirectional light created: ", lightID, "\n\n" );

    print();

    // Indicate to the GUI that a new light has been created.
    emit lightCreated( lightID, light->getName() );
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

    // Create a struct with the light's properties.
    DirectionalLightPropertiesSharedPtr lightProperties(
                new DirectionalLightProperties( directionalLightIndex, lightIndex, lightColor, glm::vec3( 0.0f, -1.0f, 0.0f ) ) );

    DrawablePtr light = DrawablePtr( new DirectionalLight( lightProperties, lightMaterial ) );

    drawablesManager_->addDrawable( lightID.getCreatorID(),
                                    light,
                                    lightID );

    // Insert the light's properties into the lights map.
    lights_.insert( std::pair< ResourceID, LightPropertiesSharedPtr >(
                        lightID,
                        lightProperties
                        ));

    log()->debug( "\n\nDirectional light created: ", lightID, "\n\n" );
    print();

    emit lightCreated( lightID, light->getName() );
}


void LightsManager::selectLight( const ResourceID lightID )
{
    LightHandlerPtr lightHandler( new LightHandler( lights_.at( lightID ), lightID, server() ) );

    lightHandler->Observable::addObserver( this );

    emit lightSelected( lightHandler );
}


void LightsManager::removeLight( ResourceID lightID )
{
    log()->debug( "LightsManager - removing ID ", lightID, "\n" );

    // Retrieve the light to be removed.
    LightPropertiesSharedPtr light = lights_.at( lightID );

    // Free the indices held by the light.
    freeLightIndices_.push( light->getBaseLightIndex() );
    switch( light->getLightType() ){
        case LightType::DIRECTIONAL_LIGHT:
            freeDirectionalLightIndices_.push( light->getLightIndex() );
        break;
    }

    // Remove the light from lights_ vector and signal it.
    lights_.erase( lightID );
}


/***
 * 4. Remote command execution
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

            lights_.at( lightCommand->getResourceID() )->setLightColor( lightCommand->getLightColor() );

            emit lightModified( lightCommand->getResourceID() );

            notifyObservers();
        }break;
        case LightCommandType::LIGHT_AMBIENT_COEFFICIENT_CHANGE:{
            const LightAmbientCoefficientChangeCommand* lightCommand =
                    dynamic_cast< const LightAmbientCoefficientChangeCommand* >( command.get() );

            lights_.at( lightCommand->getResourceID() )->setAmbientCoefficient( lightCommand->getAmbientCoefficient() );

            emit lightModified( lightCommand->getResourceID() );

            notifyObservers();
        }break;
    }
}


/***
 * 7. Updating
 ***/

void LightsManager::update()
{
    notifyObservers();
}


void LightsManager::update( ContainerAction lastContainerAction, ResourceID lastElementModified )
{
    if( lastContainerAction == ContainerAction::ELEMENT_DELETION ){
        if( lights_.count( lastElementModified ) ){
            log()->debug( "Notified, removing light\n" );
            removeLight( lastElementModified );
        }
    }

    notifyElementAction( lastContainerAction, lastElementModified );
}


/***
 * 8. Auxiliar methods
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


void LightsManager::print()
{
    log()->lock();

    log()->debug( "LightsManager - lights\n" );

    for( auto it : lights_ ){
        log()->debug( "\t", it.first, "\n" );
    }

    log()->unlock();
}


void LightsManager::highlightLight( ResourceID lightID )
{
    drawablesManager_->highlightProperty( lights_.at( lightID ).get() );
}


void LightsManager::removeHighlights()
{
    drawablesManager_->highlightProperty( nullptr );
}


/***
 * 9. Resources management
 ***/

void LightsManager::lockResource( const ResourceID& resourceID, UserID userID )
{
    (void)( resourceID );
    (void)( userID );
    // TODO: Complete.
}


void LightsManager::unlockResourcesSelection( UserID userID )
{
    (void)( userID );
    // TODO: Complete.
}


void LightsManager::deleteResourcesSelection( UserID userID )
{
    (void)( userID );
    // TODO: Complete.
}

} // namespace como
