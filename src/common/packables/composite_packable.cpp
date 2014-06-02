/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published by
 * the Free Software Foundation.
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
#include <iostream>

namespace como {


/***
 * 2. Packing and unpacking
 ***/

void* CompositePackable::pack( void* buffer ) const
{
    std::vector< PackablePair >::const_iterator it;

    for( it = packables_.begin(); it != packables_.end(); it++ ){
        buffer = it->constant->pack( buffer );
    }

    return buffer;
}


const void* CompositePackable::unpack( const void* buffer )
{
    std::vector< PackablePair >::iterator it;

    for( it = packables_.begin(); it != packables_.end(); it++ ){
        if( it->variable != nullptr ){
            buffer = it->variable->unpack( buffer );
        }else{
            buffer = it->constant->unpack( buffer );
        }
    }

    return buffer;
}


const void* CompositePackable::unpack( const void* buffer ) const
{
    std::vector< PackablePair >::const_iterator it;

    for( it = packables_.begin(); it != packables_.end(); it++ ){
        buffer = it->constant->unpack( buffer );
    }

    return buffer;
}


/***
 * 3. Getters
 ***/


PacketSize CompositePackable::getPacketSize() const
{
    PacketSize packetSize = 0;
    std::vector< PackablePair >::const_iterator it;

    for( it = packables_.begin(); it != packables_.end(); it++ ){
        packetSize += it->constant->getPacketSize();
    }

    return packetSize;
}


std::uint8_t CompositePackable::getNumberOfPackables() const
{
    return packables_.size();
}


unsigned int CompositePackable::getOffset( Packable* packable ) const
{
    unsigned int i = 0;
    unsigned int offset = 0;

    while( ( i < packables_.size() ) && ( packables_[i].constant != packable ) ){
        offset += packables_[i].constant->getPacketSize();

        i++;
    }

    if( i >= packables_.size() ){
        throw std::runtime_error( "CompositePackable::getOffset() - Packable not found" );
    }

    return offset;
}


/***
 * 4. Packables management
 ***/

void CompositePackable::addPackable( Packable* packable )
{
    packables_.push_back( PackablePair( packable ) );
}


void CompositePackable::addPackable( const Packable* packable )
{
    packables_.push_back( PackablePair( packable ) );
}


void CompositePackable::removePackables( const Packable* firstPackable, unsigned int nElements )
{
    std::vector< PackablePair >::iterator it;

    // Get a iterator to the "firstPackable" element.
    it = packables_.begin();
    while( ( it != packables_.end() ) && ( it->constant != firstPackable ) ){
        it++;
    }

    // Erase the requested elements.
    packables_.erase( it, it + nElements );
}

} // namespace como
