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
    pendingSelections_.insert( resourceID );
    sendCommandToServer( CommandConstPtr( new ResourceCommand( ResourceCommandType::RESOURCE_LOCK, localUserID(), resourceID ) ) );
}


void ResourcesOwnershipRequester::requestSelectionUnlock()
{
    CommandConstPtr selectionUnlockCommand =
            CommandConstPtr( new ResourcesSelectionCommand( ResourcesSelectionCommandType::SELECTION_UNLOCK, localUserID() ) );
    sendCommandToServer( selectionUnlockCommand );

    unlockResourcesSelection( localUserID() );
}


void ResourcesOwnershipRequester::requestSelectionDeletion()
{
    CommandConstPtr selectionDeletionCommand =
            CommandConstPtr( new ResourcesSelectionCommand( ResourcesSelectionCommandType::SELECTION_DELETION, localUserID() ) );
    sendCommandToServer( selectionDeletionCommand );

    clearResourcesSelection( localUserID() );
}

} // namespace como
