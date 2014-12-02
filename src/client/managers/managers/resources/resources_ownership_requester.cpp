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

#include "resources_ownership_requester.hpp"
#include <common/commands/resource_commands/resource_commands.hpp>
#include <common/commands/resources_selection_commands/resources_selection_command.hpp>

namespace como {


/***
 * 1. Construction
 ***/

ResourcesOwnershipRequester::ResourcesOwnershipRequester( ServerInterfacePtr server ) :
    ServerWriter( server )
{}


/***
 * 4. Ownership requests
 ***/

void ResourcesOwnershipRequester::requestResourceLock( const ResourceID &resourceID )
{
    std::pair< std::set< ResourceID >::iterator, bool > res =
        pendingSelections_.insert( resourceID );

    // Send the request to the server only if a lock request for the same
    // resource hasn't been sent already.
    if( res.second ){
        sendCommandToServer(
                    CommandConstPtr(
                        new ResourceCommand( ResourceCommandType::RESOURCE_LOCK,
                                             localUserID(),
                                             resourceID ) ) );
    }
}


void ResourcesOwnershipRequester::requestSelectionUnlock()
{
    CommandConstPtr selectionUnlockCommand =
            CommandConstPtr( new ResourcesSelectionCommand( ResourcesSelectionCommandType::SELECTION_UNLOCK, localUserID() ) );
    sendCommandToServer( std::move( selectionUnlockCommand ) );

    unlockResourcesSelection( localUserID() );
}


void ResourcesOwnershipRequester::requestSelectionDeletion()
{
    CommandConstPtr selectionDeletionCommand =
            CommandConstPtr( new ResourcesSelectionCommand( ResourcesSelectionCommandType::SELECTION_DELETION, localUserID() ) );
    sendCommandToServer( std::move( selectionDeletionCommand ) );

    clearResourcesSelection( localUserID() );
}


/***
 * 5. Ownership management
 ***/

void ResourcesOwnershipRequester::lockResource( const ResourceID& resourceID, UserID newOwner )
{
    if( newOwner == localUserID() ){
        pendingSelections_.erase( resourceID );
    }
}


/***
 * 6. Lock responses processing
 ***/

void ResourcesOwnershipRequester::processLockResponse( const ResourceID &resourceID, bool lockResponse )
{
    LOCK
    pendingSelections_.erase( resourceID );
    if( lockResponse ){
        lockResource( resourceID, localUserID() );
    }
}

} // namespace como
