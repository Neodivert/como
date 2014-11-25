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

#ifndef USER_COMMAND_HPP
#define USER_COMMAND_HPP

#include <common/commands/type_command.hpp>

namespace como {

enum class UserCommandType : std::uint8_t
{
    USER_CONNECTION,
    USER_DISCONNECTION
};
typedef PackableUint8< UserCommandType > PackableUserCommandType;


class UserCommand : public TypeCommand< UserCommandType >
{
    public:
        /***
         * 1. Construction
         ***/
        UserCommand() = delete;
        UserCommand( UserCommandType userCommandType, UserID userID );
        UserCommand( const UserCommand& b );
        UserCommand( UserCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~UserCommand() = default;


        /***
         * 3. Operators
         ***/
        UserCommand& operator=( const UserCommand& ) = delete;
        UserCommand& operator=( UserCommand&& ) = delete;
};


typedef std::shared_ptr< const UserCommand > UserCommandConstPtr;

} // namespace como

#endif // USER_COMMAND_HPP
