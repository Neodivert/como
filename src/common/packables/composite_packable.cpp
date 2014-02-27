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
    buffer = packHeader( buffer );
    buffer = packBody( buffer );

    return buffer;
}


const void* CompositePackable::unpack( const void* buffer )
{
    buffer = unpackHeader( buffer );
    buffer = unpackBody( buffer );

    return buffer;
}


void* CompositePackable::packHeader( void* buffer ) const
{
    std::vector< Packable* >::const_iterator it;

    for( it = headerPackables_.begin(); it != headerPackables_.end(); it++ ){
        buffer = (*it)->pack( buffer );
    }

    return buffer;
}


const void* CompositePackable::unpackHeader( const void* buffer )
{
    std::vector< Packable* >::iterator it;

    for( it = headerPackables_.begin(); it != headerPackables_.end(); it++ ){
        buffer = (*it)->unpack( buffer );
    }

    return buffer;
}


void* CompositePackable::packBody( void* buffer ) const
{
    std::vector< Packable* >::const_iterator it;

    for( it = bodyPackables_.begin(); it != bodyPackables_.end(); it++ ){
        buffer = (*it)->pack( buffer );
    }

    return buffer;
}


const void* CompositePackable::unpackBody( const void* buffer )
{
    std::vector< Packable* >::iterator it;

    for( it = bodyPackables_.begin(); it != bodyPackables_.end(); it++ ){
        buffer = (*it)->unpack( buffer );
    }

    return buffer;
}


/***
 * 3. Getters
 ***/


std::uint16_t CompositePackable::getPacketSize() const
{
    return getPacketHeaderSize() +
            getPacketBodySize();
}


std::uint16_t CompositePackable::getPacketHeaderSize() const
{
    std::uint16_t packetSize = 0;
    std::vector< Packable* >::const_iterator it;

    for( it = headerPackables_.begin(); it != headerPackables_.end(); it++ ){
        packetSize += (*it)->getPacketSize();
    }

    return packetSize;
}


std::uint16_t CompositePackable::getPacketBodySize() const
{
    std::uint16_t packetSize = 0;
    std::vector< Packable* >::const_iterator it;

    for( it = bodyPackables_.begin(); it != bodyPackables_.end(); it++ ){
        packetSize += (*it)->getPacketSize();
    }

    return packetSize;
}


/***
 * 4. Packables management
 ***/

void CompositePackable::addHeaderPackable( const Packable* packable )
{
    headerPackables_.push_back( packable );
}


void CompositePackable::addBodyPackable( const Packable* packable )
{
    headerPackables_.push_back( packable );
}


} // namespace como
