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

#ifndef USER_DISCONNECTION_COMMAND_HPP
#define USER_DISCONNECTION_COMMAND_HPP

#include "user_command.hpp"

namespace como {

class UserDisconnectionCommand : public UserCommand
{
    public:
        /***
         * 1. Construction
         ***/
        UserDisconnectionCommand( UserID userID = 0 );
        UserDisconnectionCommand( const UserDisconnectionCommand& ) = default;
        UserDisconnectionCommand( UserDisconnectionCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~UserDisconnectionCommand() = default;


        /***
         * 3. Operators
         ***/
        UserDisconnectionCommand& operator=( const UserDisconnectionCommand& ) = delete;
        UserDisconnectionCommand& operator=( UserDisconnectionCommand&& ) = delete;
};

} // namespace como

#endif // USER_DISCONNECTION_COMMAND_HPP
