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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "public_scene.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

PublicScene::PublicScene( LogPtr log ) :
    Scene( log )
{
}


/***
 * 2. Server connection
 ***/

bool PublicScene::connect( const char* host, const char* port, const char* userName )
{
    try{
        std::shared_ptr< const UserAcceptancePacket > userAcceptancePacket;

        // Try to connect to the server. If there is any error, the method
        // ServerInterface::connect() throws an exception.
        log_->debug( "Connecting to (", host, ":", port, ") with name [", userName, "]...\n" );
        userAcceptancePacket = server_.connect( host, port, userName );

        // Add the local user to the scene and retrieve its ID.
        addUser( std::shared_ptr< const UserConnectionCommand >( new UserConnectionCommand( *userAcceptancePacket ) ) );
        localUserID_ = userAcceptancePacket->getId();

        // Emit a signal indicating that we have connected to a scene.
        emit connectedToScene( tr( userAcceptancePacket->getSceneName() ) );

        return true;
    }catch( std::exception& ex ){
        std::cerr << ex.what() << std::endl;
        return false;
    }
}


/***
 * 3. Users administration
 ***/

void PublicScene::addUser( std::shared_ptr< const UserConnectionCommand > userConnectedCommand )
{
    // Create the new user from the given USER_CONNECTION command.
    PublicUserPtr newUser( new  PublicUser( userConnectedCommand.get() ) );

    // Insert the new user in the users vector.
    users_.insert( std::pair< unsigned int, PublicUserPtr >( userConnectedCommand->getUserID(), newUser ) );

    // Emit a UserConnectionCommand signal.
    emit userConnected( userConnectedCommand );
}


} // namespace como
