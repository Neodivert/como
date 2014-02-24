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

#include "composite_packable.hpp"

namespace como {


/***
 * 2. Packing and unpacking
 ***/

void* CompositePackable::pack( void* buffer ) const
{
    std::vector< Packable* >::const_iterator it;

    for( it = packables_.begin(); it != packables_.end(); it++ ){
        buffer = (*it)->pack( buffer );
    }

    return buffer;
}

const void* CompositePackable::unpack( const void* buffer )
{
    std::vector< Packable* >::iterator it;

    for( it = packables_.begin(); it != packables_.end(); it++ ){
        buffer = (*it)->unpack( buffer );
    }

    return buffer;
}


/***
 * 3. Getters
 ***/

std::uint16_t CompositePackable::getPacketSize() const
{
    std::uint16_t packetSize = 0;
    std::vector< Packable* >::const_iterator it;

    for( it = packables_.begin(); it != packables_.end(); it++ ){
        packetSize += (*it)->getPacketSize();
    }

    return packetSize;
}


/***
 * 4. Packables management
 ***/

void CompositePackable::addPackable( Packable* packable )
{
    packables_.push_back( packable );
}


} // namespace como
