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

#include "user_acceptance_packet.hpp"

namespace como {

/***
 * 1. Construction
 ***/

UserAcceptancePacket::UserAcceptancePacket() :
    Packet( PacketType::USER_ACCEPTED ),
    id_( 0 ),
    name_( "Unnamed" ),
    selectionColor_( {0} )
{
    addBodyPackable( &id_ );
    addBodyPackable( &name_ );
    addBodyPackable( &selectionColor_ );
}


UserAcceptancePacket::UserAcceptancePacket( const std::uint32_t& id, const char* name, const std::uint8_t* selectionColor ) :
    Packet( PacketType::USER_ACCEPTED ),
    id_( id ),
    name_( name ),
    selectionColor_( selectionColor )
{
    addBodyPackable( &id_ );
    addBodyPackable( &name_ );
    addBodyPackable( &selectionColor_ );
}


UserAcceptancePacket::UserAcceptancePacket( const UserAcceptancePacket& b ) :
    Packet( b ),
    id_( b.id_ ),
    name_( b.name_ ),
    selectionColor_( b.selectionColor_ )
{
    addBodyPackable( &id_ );
    addBodyPackable( &name_ );
    addBodyPackable( &selectionColor_ );
}


Packet* UserAcceptancePacket::clone() const
{
    return new UserAcceptancePacket( *this );
}



/***
 * 3. Getters
 ***/

std::uint32_t UserAcceptancePacket::getId() const
{
    return id_.getValue();
}


const char* UserAcceptancePacket::getName() const
{
    return name_.getValue();
}


const std::uint8_t* UserAcceptancePacket::getSelectionColor() const
{
    return selectionColor_.getValue();
}


bool UserAcceptancePacket::expectedType() const
{
    return ( Packet::getType() == PacketType::USER_ACCEPTED );
}


/***
 * 4. Setters
 ***/

void UserAcceptancePacket::setData( const std::uint32_t& id, const char* name, const std::uint8_t* selectionColor )
{
    id_ = id;
    name_ = name;
    selectionColor_ = selectionColor;
}


void UserAcceptancePacket::setId( const std::uint32_t& id )
{
    id_ = id;
}


void UserAcceptancePacket::setName( const char* name )
{
    name_ = name;
}


void UserAcceptancePacket::setSelectionColor( const std::uint8_t& r, const std::uint8_t& g, const std::uint8_t& b, const std::uint8_t& a )
{
    selectionColor_[0] = r;
    selectionColor_[1] = g;
    selectionColor_[2] = b;
    selectionColor_[3] = a;
}


/***
 * 5. Operators
 ***/

UserAcceptancePacket& UserAcceptancePacket::operator = (const UserAcceptancePacket& b )
{
    if( this != &b ){
        Packet::operator =( b );

        id_ = b.id_;
        name_ = b.name_;
        selectionColor_ = b.selectionColor_;
    }

    return *this;
}


} // namespace como
