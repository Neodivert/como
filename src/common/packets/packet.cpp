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

#include "packet.hpp"

namespace como {


/***
 * 1. Initialization and destruction
 ***/

Packet::Packet( PacketType type ) :
    type_( type )
{
}


/***
 * 2. Packing and unpacking
 ***/

char* Packet::packHeader( char* buffer ) const
{
    // Place the package type at the beginning.
    buffer[0] = static_cast< char >( type_ );

    // Return a pointer to the body info.
    return &( buffer[Packet::getPacketSize()] );
}


const char* Packet::unpackHeader( const char* buffer )
{
    // Unpack the package type.
    type_ = static_cast< PacketType >( buffer[0] );

    // Return a pointer to the body info.
    return &( buffer[Packet::getPacketSize()] );
}


/***
 * 3. Getters
 ***/

PacketType Packet::getType() const
{
    return type_;
}


std::uint16_t Packet::getPacketSize() const
{
    return sizeof( type_ );
}

} // namespace como

