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

#ifndef RESOURCES_OWNERSHIP_REQUESTER_HPP
#define RESOURCES_OWNERSHIP_REQUESTER_HPP

#include <common/managers/resources/resources_ownership_manager.hpp>
#include <client/managers/utilities/server_writer.hpp>
#include <set>

namespace como {

class ResourcesOwnershipRequester : public ResourcesOwnershipManager, public ServerWriter
{
    public:
        /***
         * 1. Construction
         ***/
        ResourcesOwnershipRequester( ServerInterfacePtr server );
        ResourcesOwnershipRequester() = delete;
        ResourcesOwnershipRequester( const ResourcesOwnershipRequester& ) = delete;
        ResourcesOwnershipRequester( ResourcesOwnershipRequester&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~ResourcesOwnershipRequester() = default;


        /***
         * 3. Operators
         ***/
        ResourcesOwnershipRequester& operator = ( const ResourcesOwnershipRequester& ) = delete;
        ResourcesOwnershipRequester& operator = ( ResourcesOwnershipRequester&& ) = delete;


        /***
         * 4. Ownership requests
         ***/
        virtual void requestResourceLock( const ResourceID& resourceID );
        void requestSelectionUnlock();
        virtual void requestSelectionDeletion();


    protected:
        /***
         * 5. Lock response processing
         ***/
        void processLockResponse( const ResourceID& resourceID, bool lockResponse );


    private:
        std::set< ResourceID > pendingSelections_;
};

} // namespace como

#endif // RESOURCES_OWNERSHIP_REQUESTER_HPP
