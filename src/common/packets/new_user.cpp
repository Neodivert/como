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

#include "new_user.hpp"
#include <cstring>

namespace como {


/***
 * 1. Initialization and destruction
 ***/

NewUser::NewUser() :
    Packet( PacketType::NEW_USER )
{
    strcpy( name_, "Unnamed" );
}


NewUser::NewUser( const char* name ) :
    Packet( PacketType::NEW_USER )
{
    strncpy( name_, name, NAME_SIZE );
}


/***
 * 2. Packing and unpacking
 ***/

char* NewUser::pack( char* buffer ) const
{
    // Pack the header into the buffer.
    buffer = Packet::pack( buffer );

    // Pack the user name into the buffer.
    packer::pack( name_, buffer, NAME_SIZE );

    // Return the updated buffer pointer.
    return buffer;
}


const char* NewUser::unpack( const char* buffer )
{
    // Pack the header into the buffer.
    buffer = Packet::unpack( buffer );

    // Check if the packet being unpacked is one of NewUser type.
    if( getType() != PacketType::NEW_USER ){
        throw std::runtime_error( "Unexpected packet" );
    }

    // Pack the user name into the buffer.
    packer::unpack( name_, buffer, NAME_SIZE );

    // Return the updated buffer pointer.
    return buffer;
}


/***
 * 2. Getters
 ***/

std::uint16_t NewUser::getPacketSize() const
{
    return Packet::getPacketSize() + NAME_SIZE * sizeof( std::uint8_t );
}


const char* NewUser::getName() const
{
    return name_;
}


/***
 * 4. Setters
 ***/

void NewUser::setName( const char* name )
{
    strcpy( name_, name );
}

} // namespace como
