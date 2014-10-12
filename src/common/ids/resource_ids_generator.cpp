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

#include "resource_ids_generator.hpp"

namespace como {


/***
 * 1. Construction
 ***/

ResourceIDsGenerator::ResourceIDsGenerator(UserID userID) :
    nextResourceID_( userID, 1 )
{}


/***
 * 3. Getters
 ***/

ResourceID ResourceIDsGenerator::generateResourceIDs( unsigned int nIDs )
{
    unsigned int i;
    ResourceID firstResourceID = nextResourceID_;

    for( i = 0; i < nIDs; i++ ){
        nextResourceID_++;
    }

    return firstResourceID;
}

} // namespace como
