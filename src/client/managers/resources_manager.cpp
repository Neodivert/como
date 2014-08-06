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

#include "resources_manager.hpp"

namespace como {


/***
 * 1. Construction
 ***/

ResourcesManager::ResourcesManager( ServerInterfacePtr server, LogPtr log ) :
    AbstractResourcesOwnershipManager( log ),
    server_( server )
{}


/***
 * 3. Lock request
 ***/

void ResourcesManager::requestResourceLock( const ResourceID& resourceID )
{
    pendingSelections_.push( resourceID );
    sendCommandToServer( CommandConstPtr( new ResourceCommand( ResourceCommandType::RESOURCE_LOCK, localUserID(), resourceID ) ) );
}


void ResourcesManager::requestSelectionUnlock()
{
    CommandConstPtr selectionUnlockCommand =
            CommandConstPtr( new ResourcesSelectionCommand( ResourcesSelectionCommandType::SELECTION_UNLOCK, localUserID() ) );
    sendCommandToServer( selectionUnlockCommand );
}


void ResourcesManager::requestSelectionDeletion()
{
    CommandConstPtr selectionDeletionCommand =
            CommandConstPtr( new ResourcesSelectionCommand( ResourcesSelectionCommandType::SELECTION_DELETION, localUserID() ) );
    sendCommandToServer( selectionDeletionCommand );
}


/***
 * 5. Server communication
 ***/

void ResourcesManager::sendCommandToServer( CommandConstPtr command )
{
    server_->sendCommand( command );
}


/***
 * 6. Server info
 ***/

UserID ResourcesManager::localUserID() const
{
    return server_->getLocalUserID();
}


ResourceID ResourcesManager::newResourceID()
{
    return server_->getNewResourceID();
}


ServerInterfacePtr ResourcesManager::server() const
{
    return server_;
}


/***
 * 7. Resource management
 ***/

void ResourcesManager::processLockResponse( const ResourceID& resourceID, bool lockResponse )
{
    if( pendingSelections_.front() == resourceID ){
        if( lockResponse ){
            lockResource( resourceID, localUserID() );
        }

        pendingSelections_.pop();
    }
}

} // namespace como
