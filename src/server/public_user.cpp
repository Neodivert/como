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

#include "public_user.hpp"

namespace como {


/***
 * 1. Initialization and destruction
 ***/

PublicUser::PublicUser( unsigned int id, const char* name, Socket socket, std::function<void (unsigned int)> removeUserCallback ) :
    id_( id ),
    socket_( std::move( socket ) ),
    removeUserCallback_( removeUserCallback ),
    nextCommand_( 0 )
{
    strncpy( name_, name, 64 );
    std::cout << "Session created (id: " << id_ << ")" << std::endl;
}


PublicUser::~PublicUser()
{
    std::cout << "Session (" << id_ << ") destroyed" << std::endl;
}


/***
 * 2. Getters
 ***/

unsigned int PublicUser::getId()
{
    return id_;
}


const char* PublicUser::getName()
{
    return name_;
}




/***
 * 3. Setters
 ***/



/***
 * 4. Socket reading
 ***/


/***
 * 5. Socket writing
 ***/

bool PublicUser::needsSceneUpdate( const CommandsList* commandsHistoric ) const
{
    return ( nextCommand_ < commandsHistoric->size() );
}


void PublicUser::sendNextSceneUpdate( const CommandsList* commandsHistoric )
{
    // Create and prepare a SCENE_UPDATE packet.
    SceneUpdate sceneUpdatePacket;
    sceneUpdatePacket.addCommands( commandsHistoric, nextCommand_, MAX_COMMANDS_PER_PACKET );

    // Get the number of commands in the packet.
    nCommandsInLastPacket_ = (std::uint8_t)( sceneUpdatePacket.getCommands()->size() );

    // Pack the previous packet and send it to the client.
    sceneUpdatePacket.send( socket_ );

    // Update the nextCommand_ index for the next SCENE_UPDATE packet.
    nextCommand_ += nCommandsInLastPacket_;
}

} // namespace como
