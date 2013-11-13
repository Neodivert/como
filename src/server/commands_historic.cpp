/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
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
 * 1. Getters
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
 * 2. Historic modification
 ***/

void CommandsHistoric::addCommand( SceneCommandConstPtr command )
{
    // Push back the given command.
    commandsMutex_.lock();
    commands_.push_back( command );
    commandsMutex_.unlock();
}


/***
 * 3. Auxiliar methods
 ***/

bool CommandsHistoric::fillSceneUpdatePacket( SceneUpdate& packet,
                                              const unsigned int firstCommand,
                                              const unsigned int nCommands ) const
{
    commandsMutex_.lock();

    // Validity check: maxCommands can't be zero.
    if( !nCommands ){
        commandsMutex_.unlock();
        return false;
    }

    // Go to the first desired command in the commands historic.
    CommandsList::const_iterator it = commands_.begin();
    std::advance( it, firstCommand );

    // Keep adding commands to the SCENE_UPDATE packet until the  packet is
    // full, or until we reach the commands historic end.
    unsigned int i = 0;
    while( ( i < nCommands ) && ( it != commands_.end() ) ){
        packet.addCommand( *it, firstCommand + i, commands_.size() );

        it++;
        i++;
    }
    commandsMutex_.unlock();

    return true;
}

} // namespace como
