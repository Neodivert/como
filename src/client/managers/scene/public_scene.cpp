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

#include "public_scene.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

PublicScene::PublicScene( LogPtr log ) :
    Scene( log )
{
}


void PublicScene::connect( const char* host, const char* port, const char* userName )
{
    try{
        //const std::uint8_t LOCAL_USER_DEFAULT_SELECTION_COLOR[4] = {
        //    255, 0, 0, 255
        //};

        // Insert the local user in the users vector.
        //addUser( userName, LOCAL_USER_DEFAULT_SELECTION_COLOR );

        // Try to connect to the server. If there is any error, the method
        // ServerInterface::connect() throws an exception.
        log_->debug( "Connecting to (", host, ":", port, ") with name [", userName, "]...\n" );
        localUserID_ = server_.connect( host, port, userName );
    }catch( std::exception& ex ){
        throw ex;
    }
}


/***
 * 2. Users administration
 ***/

void PublicScene::addUser( std::shared_ptr< const UserConnected > userConnectedCommand )
{
    log_->debug( "PublicScene::addUser - 1 (id: ", userConnectedCommand->getUserID(), ")\n" );
    // Create the new user from the given USER_CONNECTED command.
    users_.insert( std::pair< unsigned int, PublicUser >( userConnectedCommand->getUserID(), *( new  PublicUser( userConnectedCommand.get() ) ) ) );
    //users_[ userConnectedCommand->getUserID() ] = *( new  PublicUser( userConnectedCommand.get() ) );
    log_->debug( "PublicScene::addUser - 2 (id: ", userConnectedCommand->getUserID(), ")\n" );

    // Emit a UserConnected signal.
    emit userConnected( userConnectedCommand );
}


} // namespace como
