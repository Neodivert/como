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

#include "user_connected.hpp"
#include <cstring>

namespace como {

/***
 * 1. Initialization and destruction
 ***/

UserConnected::UserConnected() :
    SceneCommand( SceneCommandType::USER_CONNECTED )
{
    unsigned int i = 0;

    name_[0] = 0;
    for( ; i<4; i++ ){
        selectionColor_[i] = 0;
    }
}


UserConnected::UserConnected( const UserAccepted& userAcceptedPacket ) :
    SceneCommand( SceneCommandType::USER_CONNECTED )
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


UserConnected::UserConnected( const UserConnected& b ) :
    SceneCommand( b )
{
    const std::uint8_t* selectionColor = nullptr;
    std::cout << "User connected - Copy constructor" << std::endl;
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

char* UserConnected::pack( char* buffer ) const
{
    unsigned int i = 0;

    // Pack the command's "header".
    buffer = SceneCommand::pack( buffer );

    // Pack the command's body.
    packer::pack( name_, buffer, NAME_SIZE );
    for( ; i<4; i++ ){
        packer::pack( selectionColor_[i], buffer );
    }

    // Return the updated buffer pointer.
    return buffer;
}


const char* UserConnected::unpack( const char* buffer )
{
    unsigned int i = 0;

    // Unpack the command's "header".
    buffer = SceneCommand::unpack( buffer );

    // Unpack the command's body.
    packer::unpack( name_, buffer, NAME_SIZE );
    for( ; i<4; i++ ){
        packer::unpack( selectionColor_[i], buffer );
    }

    // Return the updated buffer pointer.
    return buffer;
}


/***
 * 3. Getters
 ***/

std::uint16_t UserConnected::getPacketSize() const
{
    return SceneCommand::getPacketSize() + NAME_SIZE + 4;
}


const char* UserConnected::getName() const
{
    return name_;
}


const std::uint8_t* UserConnected::getSelectionColor() const
{
    return selectionColor_;
}


/***
 * 4. Setters
 ***/

void UserConnected::setName( const char* name )
{
    strncpy( name_, name, NAME_SIZE );
}


void UserConnected::setSelectionColor( const std::uint8_t& r, const std::uint8_t& g, const std::uint8_t& b, const std::uint8_t& a )
{
    selectionColor_[0] = r;
    selectionColor_[1] = g;
    selectionColor_[2] = b;
    selectionColor_[3] = a;
}

} // namespace como
