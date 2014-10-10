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

#include "resources_synchronization_library.hpp"
#include <common/commands/commands.hpp>
#include <server/sync_data/texture_sync_data.hpp>

namespace como {

/***
 * 3. Command execution
 ***/

void ResourcesSynchronizationLibrary::processCommand( const Command &command )
{
    switch( command.getTarget() ){
        case CommandTarget::TEXTURE:{
            const TextureCommand& textureCommand =
                    dynamic_cast< const TextureCommand& >( command );

            if( textureCommand.getType() == TextureCommandType::TEXTURE_CREATION ){
                resourcesSyncData_[ textureCommand.textureID() ] = ResourceSyncDataPtr( new TextureSyncData( &textureCommand ) );
            }else{
                resourcesSyncData_.at( textureCommand.textureID() )->processCommand( command );
            }
        }break;
        default:
            // TODO: Complete
        break;
    }
}


} // namespace como
