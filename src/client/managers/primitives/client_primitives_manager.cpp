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

#include "client_primitives_manager.hpp"
#include <boost/filesystem.hpp>
#include <common/utilities/paths.hpp>

namespace como {

/***
 * 1. Construction
 ***/

ClientPrimitivesManager::ClientPrimitivesManager( std::string sceneName, LogPtr log ) :
    AbstractPrimitivesManager( sceneName, log )
{}


/***
 * 3. Remote command execution
 ***/

void ClientPrimitivesManager::executeRemoteCommand( PrimitiveCategoryCommandConstPtr command )
{
    switch( command->getType() ){
        case PrimitiveCategoryCommandType::PRIMITIVE_CATEGORY_CREATION:{
            const PrimitiveCategoryCreationCommand* categoryCreationCommand =
                    dynamic_cast< const PrimitiveCategoryCreationCommand* >( command.get() );

            createCategory( categoryCreationCommand->getCategoryID(), categoryCreationCommand->getCategoryName() );
        }break;
    }
}


void ClientPrimitivesManager::executeRemoteCommand( PrimitiveCommandConstPtr command )
{
    switch( command->getType() ){
        case PrimitiveCommandType::PRIMITIVE_CREATION:{
            const PrimitiveCreationCommand* primitiveCreationCommand =
                    dynamic_cast< const PrimitiveCreationCommand* >( command.get() );

            log_->debug( "Primitive file received: [", primitiveCreationCommand->getFile()->getFilePath(), "]\n" );

            registerPrimitive( primitiveCreationCommand->getPrimitiveID(),
                             primitiveCreationCommand->getPrimitiveName(),
                             primitiveCreationCommand->getCategoryID() );

            // Emit a signal indicating the primitive insertion. Include
            // primitive's name and ID in the signal.
            // TODO: Complete
            emit primitiveAdded( primitiveCreationCommand->getPrimitiveID(),
                                 getPrimitiveRelativePath( primitiveCreationCommand->getPrimitiveID() ) );
        }break;
    }
}

/*
void Scene::executeRemotePrimitiveCommand( PrimitiveCommandConstPtr command )
{
    const PrimitiveCreationCommand * primitiveCreationCommand = nullptr;
    std::string primitiveRelPath;

    switch( command->getType() ){
        case PrimitiveCommandType::PRIMITIVE_CREATION:
            // Cast to a PRIMITIVE_SELECTION command.
            primitiveCreationCommand = dynamic_cast< const PrimitiveCreationCommand* >( command.get() );

            // Debug message.


            // Build the primitives relative path, starting from SCENES_DIR/<scene name>/primitiveexecuteRemotePrimitiveCommands.
            primitiveRelPath = primitiveCreationCommand->getFile()->getFilePath()->getValue();
            primitiveRelPath = primitiveRelPath.substr( ( std::string( SCENES_DIR ) + '/' + sceneName_ + "/primitives" ).length() + 1 );

            log_->debug( "Primitive relative path: [", primitiveRelPath, "]\n" );

            // Register the new primitive.
            drawablesManager_->registerPrimitivePath( primitiveCreationCommand->getPrimitiveID(), primitiveRelPath );


        break;
    }
}
*/


} // namespace como
