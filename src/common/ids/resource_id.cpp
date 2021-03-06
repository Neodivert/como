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
#include <iostream>

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


std::string ResourceID::toString() const
{
    return std::to_string( creatorID_ ) +
           ":" +
           std::to_string( resourceIndex_ );
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

bool ResourceID::operator != (const ResourceID &b) const
{
    return ( creatorID_ != b.creatorID_ ) || ( resourceIndex_ != b.resourceIndex_ );
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


ResourceID& ResourceID::operator ++ ()
{
    resourceIndex_++;

    return *this;
}


ResourceID ResourceID::operator ++ ( int )
{
    ResourceID result( *this );

    ++(*this);

    return result;
}


/***
 * 4. Console output
 ***/

std::ostream& operator << ( std::ostream& os, const ResourceID& id )
{
    os << "(" << id.toString() << ")";

    return os;
}

} // namespace como
