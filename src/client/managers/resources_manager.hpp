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
#include <common/utilities/observable_container/observable_container.hpp>
#include <common/commands/command.hpp>
#include <common/commands/resource_commands/resource_commands.hpp>
#include <common/managers/abstract_resources_ownership_manager.hpp>

namespace como {

template <class ResourceType>
class ResourcesManager : public AbstractResourcesOwnershipManager, public ObservableContainer< ResourceID >
{
    public:
        /***
         * 1. Construction
         ***/
        ResourcesManager( ServerInterfacePtr server, LogPtr log );
        ResourcesManager() = delete;
        ResourcesManager( const ResourcesManager& ) = delete;
        ResourcesManager( ResourcesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ResourcesManager() = default;


        /***
         * 3. Getters
         ***/
        virtual std::string getResourceName( const ResourceID& resourceID ) const = 0;


        /***
         * 4. Lock request
         ***/
        void requestResourceLock( const ResourceID& resourceID );
        void requestSelectionUnlock();
        void requestSelectionDeletion();


        /***
         * 5. Operators
         ***/
        ResourcesManager& operator = ( const ResourcesManager& ) = delete;
        ResourcesManager& operator = ( ResourcesManager&& ) = delete;


    protected:
        /***
         * 6. Server communication
         ***/
        void sendCommandToServer( CommandConstPtr command );


        /***
         * 7. Server info
         ***/
        UserID localUserID() const;
        ResourceID newResourceID();
        ServerInterfacePtr server() const; // TODO: Make this method unnecessary and remove it.


        /***
         * 8. Resource management
         ***/
        virtual void lockResource( const ResourceID& resourceID, UserID userID ) = 0;
        virtual void unlockResourcesSelection( UserID userID ) = 0;
        virtual void deleteResourcesSelection( UserID userID ) = 0;
        virtual void processLockResponse( const ResourceID& resourceID, bool lockResponse );


    private:
        ServerInterfacePtr server_;

        std::queue< ResourceID > pendingSelections_;
};


/***
 * 1. Construction
 ***/

template <class ResourceType>
ResourcesManager<ResourceType>::ResourcesManager( ServerInterfacePtr server, LogPtr log ) :
    AbstractResourcesOwnershipManager( log ),
    server_( server )
{}


/***
 * 4. Lock request
 ***/

template <class ResourceType>
void ResourcesManager<ResourceType>::requestResourceLock( const ResourceID& resourceID )
{
    pendingSelections_.push( resourceID );
    sendCommandToServer( CommandConstPtr( new ResourceCommand( ResourceCommandType::RESOURCE_LOCK, localUserID(), resourceID ) ) );
}


template <class ResourceType>
void ResourcesManager<ResourceType>::requestSelectionUnlock()
{
    CommandConstPtr selectionUnlockCommand =
            CommandConstPtr( new ResourcesSelectionCommand( ResourcesSelectionCommandType::SELECTION_UNLOCK, localUserID() ) );
    sendCommandToServer( selectionUnlockCommand );
}


template <class ResourceType>
void ResourcesManager<ResourceType>::requestSelectionDeletion()
{
    CommandConstPtr selectionDeletionCommand =
            CommandConstPtr( new ResourcesSelectionCommand( ResourcesSelectionCommandType::SELECTION_DELETION, localUserID() ) );
    sendCommandToServer( selectionDeletionCommand );
}


/***
 * 6. Server communication
 ***/

template <class ResourceType>
void ResourcesManager<ResourceType>::sendCommandToServer( CommandConstPtr command )
{
    server_->sendCommand( command );
}


/***
 * 7. Server info
 ***/

template <class ResourceType>
UserID ResourcesManager<ResourceType>::localUserID() const
{
    return server_->getLocalUserID();
}


template <class ResourceType>
ResourceID ResourcesManager<ResourceType>::newResourceID()
{
    return server_->getNewResourceID();
}


template <class ResourceType>
ServerInterfacePtr ResourcesManager<ResourceType>::server() const
{
    return server_;
}


/***
 * 8. Resource management
 ***/

template <class ResourceType>
void ResourcesManager<ResourceType>::processLockResponse( const ResourceID& resourceID, bool lockResponse )
{
    if( pendingSelections_.front() == resourceID ){
        if( lockResponse ){
            lockResource( resourceID, localUserID() );
        }

        pendingSelections_.pop();
    }
}

} // namespace como

#endif // RESOURCES_MANAGER_HPP
