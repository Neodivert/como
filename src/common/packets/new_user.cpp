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

#include "new_user.hpp"
#include <cstring>

namespace como {


/***
 * 1. Initialization and destruction
 ***/

NewUser::NewUser() :
    Packet( PacketType::NEW_USER )
{
    bodySize_ += NAME_SIZE;

    strcpy( name_, "Unnamed" );
}


NewUser::NewUser( const char* name ) :
    Packet( PacketType::NEW_USER )
{
    bodySize_ += NAME_SIZE;

    strncpy( name_, name, NAME_SIZE );
}


NewUser::NewUser( const NewUser& b ) :
    Packet( b )
{
    strncpy( name_, b.name_, NAME_SIZE );
}


Packet* NewUser::clone() const
{
    return new NewUser( *this );
}


/***
 * 2. Packing and unpacking
 ***/

char* NewUser::packBody( char* buffer ) const
{
    // Pack the user name into the buffer.
    packer::pack( name_, buffer, NAME_SIZE );

    // Return the updated buffer pointer.
    return buffer;
}


const char* NewUser::unpackBody( const char* buffer )
{
    // Pack the user name into the buffer.
    packer::unpack( name_, buffer, NAME_SIZE );

    // Return the updated buffer pointer.
    return buffer;
}


/***
 * 2. Getters
 ***/

const char* NewUser::getName() const
{
    return name_;
}


bool NewUser::expectedType() const
{
    return ( Packet::getType() == PacketType::NEW_USER );
}


/***
 * 4. Setters
 ***/

void NewUser::setName( const char* name )
{
    strcpy( name_, name );
}

} // namespace como
