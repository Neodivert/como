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

#include "abstract_resources_ownership_manager.hpp"

namespace como {


/***
 * 1. Construction
 ***/

AbstractResourcesOwnershipManager::AbstractResourcesOwnershipManager( LogPtr log ) :
    log_( log )
{}


/***
 * 3. Command execution
 ***/

void AbstractResourcesOwnershipManager::executeResourceCommand( const ResourceCommand& command )
{
    LOCK
    switch( command.getType() ){
        case ResourceCommandType::RESOURCE_LOCK:
            lockResource( command.getResourceID(), command.getUserID() );
        break;
        case ResourceCommandType::RESOURCE_LOCK_DENIAL:{
            processLockResponse( command.getResourceID(), false );
        }break;
    }
}


void AbstractResourcesOwnershipManager::executeResourcesSelectionCommand( const ResourcesSelectionCommand& command )
{
    LOCK
    switch( command.getType() ){
        case ResourcesSelectionCommandType::SELECTION_UNLOCK:
            unlockResourcesSelection( command.getUserID() );
        break;
        case ResourcesSelectionCommandType::SELECTION_DELETION:
            deleteResourcesSelection( command.getUserID() );
        break;
    }
}

/***
 * 5. Getters
 ***/

LogPtr AbstractResourcesOwnershipManager::log()
{
    return log_;
}


}
