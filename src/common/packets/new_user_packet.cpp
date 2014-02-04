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

#include "new_user_packet.hpp"
#include <cstring>

namespace como {


/***
 * 1. Initialization and destruction
 ***/

NewUserPacket::NewUserPacket() :
    Packet( PacketType::NEW_USER ),
    name_{ 0 }
{
    bodySize_ += NAME_SIZE;

    strcpy( name_, "Unnamed" );
}


NewUserPacket::NewUserPacket( const char* name ) :
    Packet( PacketType::NEW_USER ),
    name_{ 0 }
{
    bodySize_ += NAME_SIZE;

    strncpy( name_, name, NAME_SIZE );
}


NewUserPacket::NewUserPacket( const NewUserPacket& b ) :
    Packet( b ),
    name_{ 0 }
{
    strncpy( name_, b.name_, NAME_SIZE );
}


Packet* NewUserPacket::clone() const
{
    return new NewUserPacket( *this );
}


/***
 * 2. Packing and unpacking
 ***/

char* NewUserPacket::packBody( char* buffer ) const
{
    // Pack the user name into the buffer.
    packer::pack( name_, buffer, NAME_SIZE );

    // Return the updated buffer pointer.
    return buffer;
}


const char* NewUserPacket::unpackBody( const char* buffer )
{
    // Pack the user name into the buffer.
    packer::unpack( name_, buffer, NAME_SIZE );

    // Return the updated buffer pointer.
    return buffer;
}


/***
 * 2. Getters
 ***/

const char* NewUserPacket::getName() const
{
    return name_;
}


bool NewUserPacket::expectedType() const
{
    return ( Packet::getType() == PacketType::NEW_USER );
}


/***
 * 4. Setters
 ***/

void NewUserPacket::setName( const char* name )
{
    strncpy( name_, name, NAME_SIZE );
}


/***
 * 5. Operators
 ***/

NewUserPacket& NewUserPacket::operator = (const NewUserPacket& b)
{
    if( this != &b ){
        Packet::operator =( b );

        strncpy( name_, b.name_, NAME_SIZE );
    }

    return *this;
}

} // namespace como
