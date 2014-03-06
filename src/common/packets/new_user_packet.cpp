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

#include "new_user_packet.hpp"
#include <cstring>

namespace como {


/***
 * 1. Initialization and destruction
 ***/

NewUserPacket::NewUserPacket() :
    Packet( PacketType::NEW_USER )
{
    name_ = "Unnamed";

    addBodyPackable( &name_ );
}


NewUserPacket::NewUserPacket( const char* name ) :
    Packet( PacketType::NEW_USER )
{
    name_ = name;

    addBodyPackable( &name_ );
}


NewUserPacket::NewUserPacket( const NewUserPacket& b ) :
    Packet( b )
{
    name_ = b.name_;

    addBodyPackable( &name_ );
}


Packet* NewUserPacket::clone() const
{
    return new NewUserPacket( *this );
}


/***
 * 3. Getters
 ***/

const char* NewUserPacket::getName() const
{
    return name_.getValue();
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
    name_ = name;
}


/***
 * 5. Operators
 ***/

NewUserPacket& NewUserPacket::operator = (const NewUserPacket& b)
{
    if( this != &b ){
        Packet::operator =( b );

        name_ = b.name_;
    }

    return *this;
}

} // namespace como
