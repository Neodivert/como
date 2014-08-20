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

#include "users_manager.hpp"

namespace como {

/***
 * 1. Construction
 ***/

UsersManager::UsersManager( const UserAcceptancePacket& userPacket ) :
    localUserID_( userPacket.getId() )
{
    UserConnectionCommand userConnectionCommand( userPacket );

    users_.emplace( userPacket.getId(), userConnectionCommand );
}


/***
 * 3. Getters
 ***/

ColouredUser UsersManager::user( UserID userID ) const
{
    return users_.at( userID );
}


ColouredUser UsersManager::localUser() const
{
    return users_.at( localUserID_ );
}


/***
 * 4. Command execution
 ***/

void UsersManager::executeRemoteCommand( const UserCommand& userCommand )
{
    switch( userCommand.getType() ){
        case UserCommandType::USER_CONNECTION:
            addUser( dynamic_cast< const UserConnectionCommand& >( userCommand ) );
        break;
        case UserCommandType::USER_DISCONNECTION:
            removeUser( userCommand.getUserID() );
        break;
        case UserCommandType::PARAMETER_CHANGE:
            // TODO: Change the ParameterChange hierarchy for distinguishing
            // those that affects selections from others.
            //entitiesManager_->executeRemoteParameterChangeCommand( dynamic_pointer_cast< const UserParameterChangeCommand >( command ) );
        break;
    }
}


/***
 * 6. Users management
 ***/

void UsersManager::addUser( const UserConnectionCommand& command )
{
    if( users_.count( command.getUserID() ) ){
        throw std::runtime_error( "UsersManager::addUser - user already created!" );
    }
    users_.emplace( command.getUserID(), command );

    notifyElementInsertion( command.getUserID() );
}


void UsersManager::removeUser( UserID userID )
{
    users_.erase( userID );

    notifyElementDeletion( userID );
}


} // namespace como
