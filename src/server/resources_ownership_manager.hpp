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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef RESOURCES_OWNERSHIP_MANAGER_HPP
#define RESOURCES_OWNERSHIP_MANAGER_HPP

#include <common/managers/abstract_resources_ownership_manager.hpp>
#include <common/utilities/observable_container/observable_container.hpp>
#include <server/public_user.hpp>
#include <map>

namespace como {

typedef std::map< ResourceID, UserID > ResourcesOwnershipMap;

class ResourcesOwnershipManager : public AbstractResourcesOwnershipManager, public ObservableContainer<ResourceID>
{
    private:
        UsersMap& users_;
        ResourcesOwnershipMap resourcesOwnershipMap_;

    public:
        /***
         * 1. Construction
         ***/
        ResourcesOwnershipManager( UsersMap& usersMap, LogPtr log );
        ResourcesOwnershipManager() = delete;
        ResourcesOwnershipManager( const ResourcesOwnershipManager& ) = delete;
        ResourcesOwnershipManager( ResourcesOwnershipManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ResourcesOwnershipManager() = default;


        /***
         * 3. Resources registation
         ***/
        virtual void registerResource( const ResourceID& resourceID, UserID ownerID );


        /***
         * 4. Operators
         ***/
        ResourcesOwnershipManager& operator = ( const ResourcesOwnershipManager& ) = delete;
        ResourcesOwnershipManager& operator = ( ResourcesOwnershipManager&& ) = delete;


        /***
         * 5. Owners management
         ***/
        void removeUser( UserID userID );


    protected:
        /***
         * 6. Resources ownership management
         ***/
        virtual void lockResource( const ResourceID& resourceID, UserID userID );
        virtual void unlockResourcesSelection( UserID userID );
        virtual void deleteResourcesSelection( UserID userID );
        virtual void processLockResponse( const ResourceID& resourceID, bool lockResponse );
};

} // namespace como

#endif // RESOURCES_OWNERSHIP_MANAGER_HPP
