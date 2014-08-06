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

#ifndef RESOURCES_MANAGER_HPP
#define RESOURCES_MANAGER_HPP

#include <client/managers/server_interface/server_interface.hpp>
#include <common/utilities/observer_pattern/observable.hpp>
#include <common/commands/command.hpp>
#include <common/commands/resource_commands/resource_commands.hpp>
#include <common/managers/abstract_resources_ownership_manager.hpp>

namespace como {

class ResourcesManager : public AbstractResourcesOwnershipManager, public Observable
{
    private:
        // TODO: Add a log_ attribute and remove it from derived classes.
        ServerInterfacePtr server_;

        std::queue< ResourceID > pendingSelections_;

    public:
        /***
         * 1. Construction
         ***/
        ResourcesManager( ServerInterfacePtr server );
        ResourcesManager() = delete;
        ResourcesManager( const ResourcesManager& ) = delete;
        ResourcesManager( ResourcesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ResourcesManager() = default;


        /***
         * 3. Lock request
         ***/
        void requestLock( const ResourceID& resourceID );


        /***
         * 4. Operators
         ***/
        ResourcesManager& operator = ( const ResourcesManager& ) = delete;
        ResourcesManager& operator = ( ResourcesManager&& ) = delete;


    protected:
        /***
         * 5. Server communication
         ***/
        void sendCommandToServer( CommandConstPtr command );


        /***
         * 6. Server info
         ***/
        UserID localUserID() const;
        ResourceID newResourceID();
        ServerInterfacePtr server() const; // TODO: Make this method unnecessary and remove it.


        /***
         * 7. Resource management
         ***/
        virtual void lockResource( const ResourceID& resourceID, UserID userID ) = 0;
        virtual void unlockResourcesSelection( UserID userID ) = 0;
        virtual void deleteResourcesSelection( UserID userID ) = 0;
        virtual void processLockResponse( const ResourceID& resourceID, bool lockResponse );
};

} // namespace como

#endif // RESOURCES_MANAGER_HPP
