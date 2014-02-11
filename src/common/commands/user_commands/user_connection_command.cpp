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

#include "user_connection_command.hpp"
#include <cstring>

namespace como {

/***
 * 1. Initialization and destruction
 ***/

UserConnectionCommand::UserConnectionCommand( UserID userID ) :
    UserCommand( UserCommandType::USER_CONNECTION, userID ),
    name_({0}),
    selectionColor_({0})

{
}


UserConnectionCommand::UserConnectionCommand( const UserAcceptancePacket& userAcceptedPacket ) :
    UserCommand( UserCommandType::USER_CONNECTION, userAcceptedPacket.getId() ),
    name_({0}),
    selectionColor_({0})
{
    const std::uint8_t* selectionColor;

    setUserID( userAcceptedPacket.getId() );
    setName( userAcceptedPacket.getName() );

    selectionColor = userAcceptedPacket.getSelectionColor();
    setSelectionColor( selectionColor[0],
                       selectionColor[1],
                       selectionColor[2],
                       selectionColor[3] );
}


UserConnectionCommand::UserConnectionCommand( const UserConnectionCommand& b ) :
    UserCommand( b ),
    name_({0}),
    selectionColor_({0})
{
    const std::uint8_t* selectionColor = nullptr;
    setUserID( b.getUserID() );
    setName( b.getName() );

    selectionColor = b.getSelectionColor();
    setSelectionColor( selectionColor[0],
                       selectionColor[1],
                       selectionColor[2],
                       selectionColor[3] );
}

/***
 * 2. Packing and unpacking
 ***/

char* UserConnectionCommand::pack( char* buffer ) const
{
    unsigned int i = 0;

    // Pack UserCommand attributes.
    buffer = UserCommand::pack( buffer );

    // Pack UserConnectionCommand attributes.
    packer::pack( name_, buffer, NAME_SIZE );
    for( i=0; i<4; i++ ){
        packer::pack( selectionColor_[i], buffer );
    }

    // Return the updated buffer pointer.
    return buffer;
}


const char* UserConnectionCommand::unpack( const char* buffer )
{
    unsigned int i = 0;

    // Unpack UserCommand attributes.
    buffer = UserCommand::unpack( buffer );

    // Unpack UserConnectionCommand attributes.
    packer::unpack( name_, buffer, NAME_SIZE );
    for( i=0; i<4; i++ ){
        packer::unpack( selectionColor_[i], buffer );
    }

    // Return the updated buffer pointer.
    return buffer;
}


/***
 * 3. Getters
 ***/


std::uint16_t UserConnectionCommand::getPacketSize() const
{
    return UserCommand::getPacketSize() + NAME_SIZE + 4;
}


const char* UserConnectionCommand::getName() const
{
    return name_;
}


const std::uint8_t* UserConnectionCommand::getSelectionColor() const
{
    return selectionColor_;
}


/***
 * 4. Setters
 ***/

void UserConnectionCommand::setName( const char* name )
{
    strncpy( name_, name, NAME_SIZE );
}


void UserConnectionCommand::setSelectionColor( const std::uint8_t& r, const std::uint8_t& g, const std::uint8_t& b, const std::uint8_t& a )
{
    selectionColor_[0] = r;
    selectionColor_[1] = g;
    selectionColor_[2] = b;
    selectionColor_[3] = a;
}

} // namespace como
