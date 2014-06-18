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

#include "packable_resource_id.hpp"

namespace como {

/***
 * 1. Construction
 ***/

PackableResourceID::PackableResourceID() :
    CompositePackable()
{
    addPackable( &creatorID_ );
    addPackable( &resourceIndex_ );
}


PackableResourceID::PackableResourceID( const ResourceID& resourceID ) :
    CompositePackable(),
    creatorID_( resourceID.getCreatorID() ),
    resourceIndex_( resourceID.getResourceIndex() )
{
    addPackable( &creatorID_ );
    addPackable( &resourceIndex_ );
}

PackableResourceID::PackableResourceID( const PackableResourceID& b ) :
    CompositePackable( b ),
    creatorID_( b.creatorID_ ),
    resourceIndex_( b.resourceIndex_ )
{
    addPackable( &creatorID_ );
    addPackable( &resourceIndex_ );
}


/***
 * 3. Getters
 ***/

ResourceID PackableResourceID::getValue() const
{
    return ResourceID( creatorID_.getValue(), resourceIndex_.getValue() );
}


/***
 * 4. Setters
 ***/

void PackableResourceID::setValue( ResourceID resourceID )
{
    creatorID_ = resourceID.getCreatorID();
    resourceIndex_ = resourceID.getResourceIndex();
}


/***
 * 5. Operators
 ***/

PackableResourceID& PackableResourceID::operator = ( const ResourceID& resourceID )
{
    setValue( resourceID );

    return *this;
}

} // namespace como
