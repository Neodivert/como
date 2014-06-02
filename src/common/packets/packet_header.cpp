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

#include "packet_header.hpp"

namespace como {

/***
 * 1. Construction
 ***/

PacketHeader::PacketHeader( PacketType type ) :
    type_( type ),
    bodySize_( 0 )
{
    addPackable( &type_ );
    addPackable( &bodySize_ );
}


PacketHeader::PacketHeader( const PacketHeader& b ) :
    CompositePackable( b ),
    type_( b.type_ ),
    bodySize_( b.bodySize_ )
{
    addPackable( &type_ );
    addPackable( &bodySize_ );
}


/***
 * 3. Getters
 ***/

PacketType PacketHeader::getType() const
{
    return type_.getValue();
}


PacketSize PacketHeader::getBodySize() const
{
    return bodySize_.getValue();
}


/***
 * 4. Setters
 ***/

void PacketHeader::setBodySize( PacketSize bodySize )
{
    bodySize_ = bodySize;
}


/***
 * 5. Operators
 ***/

CompositePackable& PacketHeader::operator = (const PacketHeader& b )
{
    if( this != &b ){
        type_ = b.type_;
        bodySize_ = b.bodySize_;
    }

    return *this;
}


CompositePackable& PacketHeader::operator = ( PacketHeader&& b )
{
    if( this != &b ){
        type_ = b.type_;
        bodySize_ = b.bodySize_;
    }

    return *this;
}


} // namespace como
