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

#ifndef USER_CONNECTION_COMMAND_HPP
#define USER_CONNECTION_COMMAND_HPP

#include <common/packets/user_acceptance_packet.hpp>
#include "user_command.hpp"


namespace como {

class UserConnectionCommand : public UserCommand
{
    private:
        PackableString name_;
        PackableColor selectionColor_;

    public:
        /***
         * 1. Construction
         ***/
        UserConnectionCommand( UserID userID = 0 );
        UserConnectionCommand( const UserAcceptancePacket& userAcceptancePacket );
        UserConnectionCommand( const UserConnectionCommand& b );
        UserConnectionCommand( UserConnectionCommand&& ) = delete;
        COMMAND_CLONE_METHOD( UserConnectionCommand )


        /***
         * 2. Destruction
         ***/
        virtual ~UserConnectionCommand() = default;


        /***
         * 3. Getters
         ***/
        const char* getName() const ;
        Color getSelectionColor() const ;


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
