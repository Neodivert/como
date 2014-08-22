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

#ifndef RESOURCES_OWNERSHIP_MANAGER_HPP
#define RESOURCES_OWNERSHIP_MANAGER_HPP

#include <common/ids/resource_id.hpp>

namespace como {

class ResourcesOwnershipManager {
    friend class EntitiesManager; // FIXME

    public:
        /***
         * 1. Construction
         ***/
        ResourcesOwnershipManager() = default;
        ResourcesOwnershipManager( const ResourcesOwnershipManager& ) = delete;
        ResourcesOwnershipManager( ResourcesOwnershipManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ResourcesOwnershipManager() = default;


        /***
         * 3. Operators
         ***/
        ResourcesOwnershipManager& operator = ( const ResourcesOwnershipManager& ) = delete;
        ResourcesOwnershipManager& operator = ( ResourcesOwnershipManager&& ) = delete;


    protected:
        /***
         * 4. Resources ownership management
         ***/
        virtual void lockResource( const ResourceID& resourceID, UserID newOwner ) = 0;
        virtual void unlockResourcesSelection( UserID currentOwner ) = 0;
        virtual void clearResourcesSelection( UserID currentOwner ) = 0;
};

} // namespace como

#endif // RESOURCES_OWNERSHIP_MANAGER_HPP
