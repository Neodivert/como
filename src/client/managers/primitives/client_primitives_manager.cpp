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

ClientPrimitivesManager::ClientPrimitivesManager( std::string sceneName, ServerInterfacePtr server, LogPtr log ) :
    AbstractPrimitivesManager( sceneName, log ),
    server_( server )
{}


/***
 * 3. Primitives management
 ***/

void ClientPrimitivesManager::createPrimitive( std::string filePath, ResourceID categoryID )
{
    std::string name =
            boost::filesystem::basename( filePath ) +
            boost::filesystem::extension( filePath );

    ResourceID id = server_->getNewResourceID();

    registerPrimitive( id, name, categoryID );

    // TODO: What if the file already exists in the destination dir?
    boost::filesystem::copy( filePath, getPrimitiveAbsolutePath( id ) );

    emit primitiveAdded( id, getPrimitiveRelativePath( id ) );

    // TODO: Send command to server.
}


/***
 * 4. Remote command execution
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

} // namespace como
