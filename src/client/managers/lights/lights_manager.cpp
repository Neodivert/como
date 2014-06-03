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
{}


/***
 * 3. Lights management
 ***/

void LightsManager::addDirectionalLight( const LightID& lightID, const PackableColor& lightColor )
{
    MaterialConstPtr lightMaterial( new Material( PackableColor( 255, 0, 0, 255 ) ) );

    drawablesManager_->addDrawable( lightID.creatorID.getValue(),
                                    DrawablePtr( new DirectionalLight( lightMaterial, lightColor ) ),
                                    lightID );
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

} // namespace como
