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

LightsManager::LightsManager( DrawablesManagerPtr drawablesManager, ServerInterfacePtr server ) :
    drawablesManager_( drawablesManager ),
    server_( server )
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

    PackableColor lightColor( 255, 0, 0, 255 );

    // Create a light with the previous material and the given light color.
    DrawablePtr light = DrawablePtr( new DirectionalLight( directionalLightIndex, lightIndex, lightMaterial, lightColor ) );

    // Add the created light to the Drawables Manager and retrieve the ID given
    // to it.
    LightID lightID = drawablesManager_->addDrawable( light );

    lights_.insert( std::pair< LightID, LightPtr >(
                        lightID,
                        dynamic_pointer_cast< Light >( light )
                        ));

    server_->sendCommand( CommandConstPtr( new DirectionalLightCreationCommand( lightID, lightColor ) ) );

    // Indicate to the GUI that a new light has been created.
    emit lightCreated( lightID, light->getName() );
}


void LightsManager::addDirectionalLight( const LightID& lightID, const PackableColor& lightColor )
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

    DrawablePtr light = DrawablePtr( new DirectionalLight( directionalLightIndex, lightIndex, lightMaterial, lightColor ) );

    drawablesManager_->addDrawable( lightID.creatorID.getValue(),
                                    light,
                                    lightID );

    lights_.insert( std::pair< LightID, LightPtr >(
                        lightID,
                        dynamic_pointer_cast< Light >( light )
                        ));

    emit lightCreated( lightID, light->getName() );
}


void LightsManager::selectLight( const LightID lightID )
{
    emit lightSelected( LightHandlerPtr( new LightHandler( lights_.at( lightID ), lightID, server_, std::bind( &LightsManager::setChanged, this ) ) ) );
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

                    addDirectionalLight( directionalLightCreationCommand->getLightID(),
                                         directionalLightCreationCommand->getLightColor()
                                         );
                }break;
            }
        }break;
    }
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

} // namespace como
