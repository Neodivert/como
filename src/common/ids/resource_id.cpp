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

#include "resource_id.hpp"

namespace como {


/***
 * 1. Construction
 ***/

ResourceID::ResourceID() :
    creatorID_( 0 ),
    resourceIndex_( 0 )
{}

ResourceID::ResourceID(UserID creatorID, ResourceIndex resourceIndex) :
    creatorID_( creatorID ),
    resourceIndex_( resourceIndex )
{}


ResourceID::ResourceID( const ResourceID& b ) :
    creatorID_( b.creatorID_ ),
    resourceIndex_( b.resourceIndex_ )
{}


ResourceID::ResourceID( ResourceID&& b ) :
    creatorID_( b.creatorID_ ),
    resourceIndex_( b.resourceIndex_ )
{}


/***
 * 3. Getters
 ***/

UserID ResourceID::getCreatorID() const
{
    return creatorID_;
}

ResourceIndex ResourceID::getResourceIndex() const
{
    return resourceIndex_;
}


/***
 * 4. Operators
 ***/


bool ResourceID::operator < ( const ResourceID& b ) const
{
    return ( ( creatorID_ < b.creatorID_ ) ||
             ( (creatorID_ == b.creatorID_) && (resourceIndex_ < b.resourceIndex_) ) );
}


bool ResourceID::operator == ( const ResourceID& b ) const
{
    return ( ( creatorID_ == b.creatorID_ ) &&
             (resourceIndex_ == b.resourceIndex_ ) );
}


ResourceID& ResourceID::operator = ( const ResourceID& b )
{
    if( this != &b ){
        creatorID_ = b.creatorID_;
        resourceIndex_ = b.resourceIndex_;
    }

    return *this;
}


ResourceID& ResourceID::operator = ( ResourceID&& b )
{
    if( this != &b ){
        creatorID_ = b.creatorID_;
        resourceIndex_ = b.resourceIndex_;
    }

    return *this;
}


/***
 * 4. Console output
 ***/

std::ostream& operator << ( std::ostream& os, const ResourceID& id )
{
    os << "(" << static_cast< int >( id.getCreatorID() ) << ", "
       << static_cast<int>( id.getResourceIndex() ) << ")";

    return os;
}

} // namespace como
