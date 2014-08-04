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

#ifndef RESOURCE_ID_HPP
#define RESOURCE_ID_HPP

#include <common/packables/packable_integer.hpp>
#include "user_id.hpp"
#include <cstdint>

namespace como {

typedef std::uint32_t ResourceIndex;
typedef PackableUint32< ResourceIndex > PackableResourceIndex;


/*
 * A resource is uniquely identified by a pair of values:
 * - The ID of the user who created it.
 * - A index to differ the resource from others created by the same user.
 */
class ResourceID
{
    private:
        UserID creatorID_;
        ResourceIndex resourceIndex_;

    public:
        /***
         * 1. Construction
         ***/
        ResourceID();
        ResourceID( UserID creatorID, ResourceIndex resourceIndex );
        ResourceID( const ResourceID& b ) = default;
        ResourceID( ResourceID&& b ) = default;


        /***
         * 2. Destruction
         ***/
        ~ResourceID() = default;


        /***
         * 3. Getters
         ***/
        UserID getCreatorID() const;
        ResourceIndex getResourceIndex() const;


        /***
         * 4. Operators
         ***/
        bool operator < ( const ResourceID& b ) const;
        bool operator == ( const ResourceID& b ) const;

        ResourceID& operator = (const ResourceID& b );
        ResourceID& operator = ( ResourceID&& );

        ResourceID& operator ++ (); // Prefix
        ResourceID operator ++ ( int ); // Postfix
};

const ResourceID NO_RESOURCE( 0, 0 );


/***
 * 4. Console output
 ***/

std::ostream& operator << ( std::ostream& os, const ResourceID& id );

} // namespace como

#endif // RESOURCE_ID_HPP
