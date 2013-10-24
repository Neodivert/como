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

#include "user_accepted.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

UserAccepted::UserAccepted() :
    Packet( PacketType::USER_ACCEPTED ),
    id_( 0 )
{
    unsigned int i = 0;

    strcpy( name_, "Unnamed" );

    for( ; i<4; i++ ){
        selectionColor_[i] = 0;
    }
}


UserAccepted::UserAccepted( const std::uint32_t& id, const char* name, const std::uint8_t* selectionColor ) :
    Packet( PacketType::USER_ACCEPTED )
{
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


/***
 * 1. Packing and unpacking
 ***/

void UserAccepted::pack( char* buffer ) const
{
    unsigned int i = 0;
    std::uint8_t* colorPtr = nullptr;

    // Place the packet's header at the beginning of the buffer.
    buffer = packHeader( buffer );

    // Place the user's id into the buffer.
    ( reinterpret_cast< std::uint32_t* >( buffer ) )[0] = translateToNetworkOrder( id_ );

    // Place the user's name after the id in the stream.
    strncpy( &buffer[4], name_, NAME_SIZE );

    // Place the user's selection color at the end.
    colorPtr = reinterpret_cast< std::uint8_t* >( &buffer[4+NAME_SIZE] );
    for( ; i < 4; i++ ){
        *colorPtr = selectionColor_[i];

        colorPtr++;
    }
}


void UserAccepted::unpack( const char* buffer )
{
    const std::uint8_t* colorPtr = nullptr;
    unsigned int i = 0;

    // Unpack the packet's header.
    buffer = unpackHeader( buffer );

    // Check if the packet's type is the expected one.
    if( getType() != PacketType::USER_ACCEPTED ){
        throw std::runtime_error( std::string( "Unexpected packet" ) );
    }

    // Get the user's id.
    id_ = translateFromNetworkOrder( ( reinterpret_cast< const std::uint32_t* >( buffer ) )[0] );

    // Get the user's name.
    strncpy( name_, &buffer[4], NAME_SIZE );

    // Get the user's selection color.
    colorPtr = reinterpret_cast< const std::uint8_t* >( &buffer[4+NAME_SIZE] );
    for( ; i<4; i++ ){
        selectionColor_[i] = *colorPtr;

        colorPtr++;
    }
}


/***
 * 2. Getters
 ***/

std::uint16_t UserAccepted::getPacketSize() const
{
    return Packet::getPacketSize() + sizeof( id_ ) + sizeof( char ) * NAME_SIZE + sizeof( uint8_t ) * 4;
}


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
