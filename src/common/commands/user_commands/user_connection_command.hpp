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

#ifndef USER_CONNECTION_COMMAND_HPP
#define USER_CONNECTION_COMMAND_HPP

#include "user_command.hpp"
#include "../../packets/user_acceptance_packet.hpp"

namespace como {

class UserConnectionCommand : public UserCommand
{
    private:
        PackableString<NAME_SIZE> name_;
        PackableRGBA selectionColor_;

    public:
        /***
         * 1. Construction
         ***/
        UserConnectionCommand( UserID userID = 0 );
        UserConnectionCommand( const UserAcceptancePacket& userAcceptancePacket );
        UserConnectionCommand( const UserConnectionCommand& b );
        UserConnectionCommand( UserConnectionCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~UserConnectionCommand() = default;


        /***
         * 3. Getters
         ***/
        const char* getName() const ;
        const std::uint8_t* getSelectionColor() const ;


        /***
         * 4. Setters
         ***/
        void setName( const char* name );
        void setSelectionColor( const std::uint8_t& r, const std::uint8_t& g, const std::uint8_t& b, const std::uint8_t& a );


        /***
         * 5. Operators
         ***/
        UserConnectionCommand& operator=( const UserConnectionCommand& ) = delete;
        UserConnectionCommand& operator=( UserConnectionCommand&& ) = delete;
};

typedef std::shared_ptr< const UserConnectionCommand > UserConnectionCommandConstPtr;

} // namespace como

#endif // USER_CONNECTION_COMMAND_HPP
