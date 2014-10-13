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

#ifndef RESOURCE_IDS_GENERATOR_HPP
#define RESOURCE_IDS_GENERATOR_HPP

#include "resource_id.hpp"
#include <memory>
#include <common/utilities/lockable.hpp>

namespace como {

class ResourceIDsGenerator : public Lockable
{
    public:
        /***
         * 1. Construction
         ***/
        ResourceIDsGenerator( UserID userID );
        ResourceIDsGenerator() = delete;
        ResourceIDsGenerator( const ResourceIDsGenerator& ) = delete;
        ResourceIDsGenerator( ResourceIDsGenerator&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ResourceIDsGenerator() = default;


        /***
         * 3. Getters
         ***/
        ResourceID generateResourceIDs( unsigned int nIDs );
        UserID userID() const;


        /***
         * 4. Operators
         ***/
        ResourceIDsGenerator& operator = ( const ResourceIDsGenerator& ) = delete;
        ResourceIDsGenerator& operator = ( ResourceIDsGenerator&& ) = delete;

    private:
        ResourceID nextResourceID_;
};

typedef std::shared_ptr< ResourceIDsGenerator > ResourceIDsGeneratorPtr;

} // namespace como

#endif // RESOURCE_IDS_GENERATOR_HPP
