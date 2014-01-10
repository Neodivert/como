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

#include "user_accepted.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

UserAccepted::UserAccepted() :
    Packet( PacketType::USER_ACCEPTED ),
    id_( 0 )
{
    bodySize_ = sizeof( id_ ) + NAME_SIZE + 4;

    unsigned int i = 0;

    strcpy( name_, "Unnamed" );

    for( ; i<4; i++ ){
        selectionColor_[i] = 0;
    }
}


UserAccepted::UserAccepted( const std::uint32_t& id, const char* name, const std::uint8_t* selectionColor ) :
    Packet( PacketType::USER_ACCEPTED )
{
    bodySize_ = sizeof( id_ ) + NAME_SIZE + 4;

    setData( id, name, selectionColor );
}


void UserAccepted::setData( const std::uint32_t& id, const char* name, const std::uint8_t* selectionColor )
{
    unsigned int i = 0;

    id_ = id;
    strcpy( name_, name );
    for( ; i<4; i++ ){
        selectionColor_[i] = selectionColor[i];
    }
}


UserAccepted::UserAccepted( const UserAccepted& b ) :
    Packet( b )
{
    setData( b.id_, b.name_, b.selectionColor_ );
}


Packet* UserAccepted::clone() const
{
    return new UserAccepted( *this );
}


/***
 * 2. Packing and unpacking
 ***/

char* UserAccepted::packBody( char* buffer ) const
{
    unsigned int i = 0;

    // Place the user's id into the buffer.
    packer::pack( id_, buffer );

    // Place the user's name after the id in the stream.
    packer::pack( name_, buffer, NAME_SIZE );

    // Place the user's selection color at the end.
    for( ; i < 4; i++ ){
        packer::pack( selectionColor_[i], buffer );
    }

    // Return the updated buffer pointer.
    return buffer;
}


const char* UserAccepted::unpackBody( const char* buffer )
{
    unsigned int i = 0;

    // Get the user's id.
    packer::unpack( id_, buffer );

    // Get the user's name.
    packer::unpack( name_, buffer, NAME_SIZE );

    // Get the user's selection color.
    for( ; i<4; i++ ){
        packer::unpack( selectionColor_[i], buffer );
    }

    // Return the updated buffer pointer.
    return buffer;
}


/***
 * 3. Getters
 ***/

std::uint32_t UserAccepted::getId() const
{
    return id_;
}


const char* UserAccepted::getName() const
{
    return name_;
}


const std::uint8_t* UserAccepted::getSelectionColor() const
{
    return selectionColor_;
}


bool UserAccepted::expectedType() const
{
    return ( Packet::getType() == PacketType::USER_ACCEPTED );
}


/***
 * 4. Setters
 ***/


void UserAccepted::setId( const std::uint32_t& id )
{
    id_ = id;
}


void UserAccepted::setName( const char* name )
{
    strcpy( name_, name );
}


void UserAccepted::setSelectionColor( const std::uint8_t& r, const std::uint8_t& g, const std::uint8_t& b, const std::uint8_t& a )
{
    selectionColor_[0] = r;
    selectionColor_[1] = g;
    selectionColor_[2] = b;
    selectionColor_[3] = a;
}

} // namespace como