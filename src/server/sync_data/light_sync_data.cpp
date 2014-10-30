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

#include "light_sync_data.hpp"

namespace como {

/***
 * 1. Construction
 ***/

LightSyncData::LightSyncData( const DirectionalLightCreationCommand &creationCommand ) :
    EntitySyncData( &creationCommand,
                    creationCommand.getResourceID(),
                    glm::vec3( 0.0f ) )
{
    light_.color = creationCommand.getLightColor().toVec3();
    // TODO: Sync ambient coefficient?
}


/***
 * 3. Getters
 ***/

std::list<CommandConstPtr> LightSyncData::generateUpdateCommands() const
{
    std::list<CommandConstPtr> updateCommands =
            EntitySyncData::generateUpdateCommands();

    updateCommands.push_back(
                CommandConstPtr(
                    new LightColorChangeCommand(
                        NO_USER,
                        resourceID(),
                        light_.color
                        ) ) );

    updateCommands.push_back(
                CommandConstPtr(
                    new LightAmbientCoefficientChangeCommand(
                        NO_USER,
                        resourceID(),
                        light_.ambientCoefficient
                        ) ) );

    return updateCommands;
}


/***
 * 4. Updating
 ***/

void LightSyncData::processCommand( const Command &command )
{
    if( command.getTarget() == CommandTarget::LIGHT ){
        const LightCommand& lightCommand =
                dynamic_cast< const LightCommand& >( command );

        switch( lightCommand.getType() ){
            case LightCommandType::LIGHT_COLOR_CHANGE:{
                const LightColorChangeCommand& colorChangeCommand =
                        dynamic_cast< const LightColorChangeCommand& >( command );
                light_.color =
                        colorChangeCommand.getLightColor().toVec3();
            }break;
            case LightCommandType::LIGHT_AMBIENT_COEFFICIENT_CHANGE:{
                const LightAmbientCoefficientChangeCommand& ambientCoeffChangeCommand =
                        dynamic_cast< const LightAmbientCoefficientChangeCommand& >( command );
                light_.ambientCoefficient =
                        ambientCoeffChangeCommand.getAmbientCoefficient();
            }break;
            default:
                throw std::runtime_error( "Unexpected light command received in LightSyncData::processCommand()" );
            break;
        }
    }else{
        EntitySyncData::processCommand( command );
    }
}

} // namespace como
