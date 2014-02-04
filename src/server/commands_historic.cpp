/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "commands_historic.hpp"

namespace como {


/***
 * 1. Construction
 ***/

CommandsHistoric::CommandsHistoric( std::function< void () > broadcastCallback ) :
    broadcastCallback_( broadcastCallback )
{
}

/***
 * 3. Getters
 ***/

unsigned int CommandsHistoric::getSize() const
{
    unsigned int size;

    // Retrieve the historic's size.
    commandsMutex_.lock();
    size = commands_.size();
    commandsMutex_.unlock();

    // Return the historic's size.
    return size;
}


/***
 * 4. Historic modification
 ***/

void CommandsHistoric::addCommand( SceneCommandConstPtr command )
{
    commandsMutex_.lock();

    // Push back the given command.
    commands_.push_back( command );

    commandsMutex_.unlock();

    // Broadcast the added command.
    broadcastCallback_();
}


/***
 * 5. Auxiliar methods
 ***/

std::uint32_t CommandsHistoric::fillSceneUpdatePacketPacket( SceneUpdatePacket& packet,
                                                       const unsigned int firstCommand,
                                                       const unsigned int nCommands,
                                                       UserID userID ) const
{
    commandsMutex_.lock();

    /*
    // Validity check: maxCommands can't be zero.
    if( !nCommands ){
        commandsMutex_.unlock();
        return false;
    }
    */

    // Go to the first desired command in the commands historic.
    CommandsList::const_iterator it = commands_.begin();
    std::advance( it, firstCommand );

    // Keep adding commands to the SCENE_UPDATE packet until the  packet is
    // full, or until we reach the commands historic end.
    uint32_t i = 0;
    uint32_t nextCommand = firstCommand;
    while( ( i < nCommands ) && ( it != commands_.end() ) ){
        // Don't send to the user its own commands.
        // TODO: Substract these commands from unsyncCommands field.
        if( (*it)->getUserID() != userID ){
            packet.addCommand( *it, firstCommand + i, commands_.size() );
            i++;
        }
        nextCommand++;
        it++;
    }
    commandsMutex_.unlock();

    return nextCommand;
}

} // namespace como
