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

#include "packable_resource_header.hpp"

namespace como {


/***
 * 1. Construction
 ***/

PackableResourceHeader::PackableResourceHeader()
{
    addPackable( &id_ );
    addPackable( &name_ );
}


PackableResourceHeader::PackableResourceHeader( const ResourceHeader &header ) :
    id_( header.id ),
    name_( header.name )
{
    addPackable( &id_ );
    addPackable( &name_ );
}


PackableResourceHeader::PackableResourceHeader(const PackableResourceHeader &b ) :
    AbstractPackableWrapper( b ),
    CompositePackable( b ),
    id_( b.id_ ),
    name_( b.name_ )
{
    addPackable( &id_ );
    addPackable( &name_ );
}


/***
 * 3. Getters
 ***/

ResourceHeader PackableResourceHeader::getValue() const
{
    return ResourceHeader( id_.getValue(),
                           name_.getValue() );
}


PacketSize PackableResourceHeader::getPacketSize() const
{
    return CompositePackable::getPacketSize();
}


/***
 * 4. Setters
 ***/

void PackableResourceHeader::setValue(const ResourceHeader &header )
{
    id_ = header.id;
    name_ = header.name;
}


/***
 * 5. Packing / unpacking
 ***/

void *PackableResourceHeader::pack(void *buffer) const
{
    return CompositePackable::pack( buffer );
}


const void *PackableResourceHeader::unpack(const void *buffer)
{
    return CompositePackable::unpack( buffer );
}


const void *PackableResourceHeader::unpack(const void *buffer) const
{
    return CompositePackable::unpack( buffer );
}

} // namespace como
