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

#ifndef USER_COMMAND_HPP
#define USER_COMMAND_HPP

#include "../command.hpp"

namespace como {

enum class UserCommandType : std::uint8_t
{
    USER_CONNECTION,
    USER_DISCONNECTION,
    PARAMETER_CHANGE
};


class UserCommand : public Command
{
    private:
        const UserCommandType commandType_;

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
        ~UserCommand() = default;


        /***
         * 2. Packing and unpacking
         ***/
        virtual char* pack( char* buffer ) const = 0;
        virtual const char* unpack( const char* buffer ) = 0;


        /***
         * 3. Getters
         ***/
        UserCommandType getType() const;
        virtual std::uint16_t getPacketSize() const ;


        /***
         * 4. Buffer pre reading
         ***/
        static UserCommandType getType( const char* buffer );


        /***
         * 5. Operators
         ***/
        UserCommand& operator=( const UserCommand& ) = delete;
        UserCommand& operator=( UserCommand&& ) = delete;
};

} // namespace como

#endif // USER_COMMAND_HPP
