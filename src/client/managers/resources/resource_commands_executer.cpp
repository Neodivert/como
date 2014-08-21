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

#include "resource_commands_executer.hpp"

namespace como {

/***
 * 3. Command execution
 ***/


void ResourceCommandsExecuter::executeResourceCommand( const ResourceCommand& command )
{
    switch( command.getType() ){
        case ResourceCommandType::RESOURCE_LOCK:
            lockResource( command.getResourceID(), command.getUserID() );
        break;
        case ResourceCommandType::RESOURCE_SELECTION_RESPONSE:{
            const ResourceSelectionResponse& responseCommand = dynamic_cast< const ResourceSelectionResponse& >( command );
            processLockResponse( responseCommand.getResourceID(), responseCommand.getResponse() );
        }break;
    }
}


void ResourceCommandsExecuter::executeResourcesSelectionCommand( const ResourcesSelectionCommand& command )
{
    switch( command.getType() ){
        case ResourcesSelectionCommandType::SELECTION_UNLOCK:
            unlockResourcesSelection( command.getUserID() );
        break;
        case ResourcesSelectionCommandType::SELECTION_DELETION:
            clearResourcesSelection( command.getUserID() );
        break;
    }
}


} // namespace como
