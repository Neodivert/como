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

#ifndef USERS_MANAGER_HPP
#define USERS_MANAGER_HPP

#include <map>
#include <client/models/users/coloured_user.hpp>
#include <common/commands/user_commands/user_command.hpp>
#include <common/packets/user_acceptance_packet.hpp>
#include <common/utilities/observable_container/observable_container.hpp>

namespace como {

class UsersManager : public ObservableContainer< UserID >
{
    public:
        /***
         * 1. Construction
         ***/
        UsersManager( const UserAcceptancePacket& userPacket );
        UsersManager() = delete;
        UsersManager( const UsersManager& ) = delete;
        UsersManager( UsersManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~UsersManager() = default;


        /***
         * 3. Getters
         ***/
        ColouredUser user( UserID userID ) const;
        ColouredUser localUser() const;


        /***
         * 4. Command execution
         ***/
        void executeRemoteCommand( const UserCommand& userCommand );


        /***
         * 5. Operators
         ***/
        UsersManager& operator = ( const UsersManager& ) = delete;
        UsersManager& operator = ( UsersManager&& ) = delete;


    protected:
        /***
         * 6. Users management
         ***/
        void addUser( const UserConnectionCommand& command );
        void removeUser( UserID userID );


    private:
        std::map< UserID, ColouredUser > users_;
        const UserID localUserID_;
};

typedef std::shared_ptr< UsersManager > UsersManagerPtr;

} // namespace como

#endif // USERS_MANAGER_HPP
