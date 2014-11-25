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
#include <common/primitives/primitive_file.hpp>

namespace como {

/***
 * 1. Construction
 ***/

ClientPrimitivesManager::ClientPrimitivesManager( std::string sceneDirPath, std::string sceneTempDirPath, ServerInterfacePtr server, MeshesManagerPtr meshesManager, LogPtr log ) :
    AbstractPrimitivesManager( sceneDirPath, sceneTempDirPath, log ),
    server_( server ),
    meshesManager_( meshesManager )
{}


/***
 * 3. Primitives management
 ***/

std::string ClientPrimitivesManager::createPrimitive( std::string filePath, ResourceID categoryID )
{
    LOCK
    ResourceID id = importMeshFile( filePath, categoryID );

    log_->debug( "Primitive imported. Sending command\n" );

    server_->sendCommand(
                CommandConstPtr(
                    new PrimitiveCreationCommand( id.getCreatorID(),
                                                  id,
                                                  getPrimitiveInfo( id ),
                                                  tempDirPath_
                                                  ) ) );

    emit primitiveAdded( id, getPrimitiveRelativePath( id ) );

    return getPrimitiveFilePath( id );
}


ResourceID ClientPrimitivesManager::importMeshFile( std::string srcFilePath, ResourceID categoryID )
{
    LOCK
    OBJPrimitivesImporter primitivesImporter;
    PrimitiveInfo primitive;
    char primitiveNameSuffix[64];

    // Generate the primitive name.
    ResourceID primitiveID = server_->reserveResourceIDs( 1 );
    sprintf( primitiveNameSuffix, "_%u_%u",
             primitiveID.getCreatorID(),
             primitiveID.getResourceIndex() );

    primitive = primitivesImporter.importPrimitive( srcFilePath, getCategoryAbsoluteePath( categoryID ), primitiveNameSuffix );
    primitive.category = categoryID;
    registerPrimitive( primitiveID, primitive );

    return primitiveID;
}


void ClientPrimitivesManager::instantiatePrimitive( ResourceID primitiveID )
{
    LOCK
    ImportedPrimitiveData primitiveData;
    primitiveData.importFromFile( getPrimitiveFilePath( primitiveID ) );

    const ResourceID meshID = server_->reserveResourceIDs( 1 );
    const ResourceID firstMaterialID = server_->reserveResourceIDs( primitiveData.materialsInfo_.size() );

    glm::vec3 meshCentroid = meshesManager_->createMesh( primitiveData, meshID, firstMaterialID );

    // Send the command to the server.
    server_->sendCommand(
                CommandConstPtr(
                    new PrimitiveInstantiationCommand(
                        server_->getLocalUserID(),
                        primitiveID,
                        meshID,
                        firstMaterialID,
                        meshCentroid ) ) );
}


// FIXME: Duplicated code.
void ClientPrimitivesManager::instantiatePrimitive( UserID userID, ResourceID primitiveID, ResourceID meshID, ResourceID firstMaterialID )
{
    LOCK
    (void)( userID );

    ImportedPrimitiveData primitiveData;

    primitiveData.importFromFile( getPrimitiveFilePath( primitiveID ) );

    meshesManager_->createMesh( primitiveData, meshID, firstMaterialID );
}


/***
 * 4. Remote command execution
 ***/

void ClientPrimitivesManager::executeRemoteCommand( const PrimitiveCategoryCommand& command )
{
    LOCK
    switch( command.getType() ){
        case PrimitiveCategoryCommandType::PRIMITIVE_CATEGORY_CREATION:{
            const PrimitiveCategoryCreationCommand& categoryCreationCommand =
                    dynamic_cast< const PrimitiveCategoryCreationCommand& >( command );

            createCategory( categoryCreationCommand.getCategoryID(), categoryCreationCommand.getCategoryName() );
        }break;
    }
}


void ClientPrimitivesManager::executeRemoteCommand( const PrimitiveCommand& command )
{
    LOCK
    switch( command.getType() ){
        case PrimitiveCommandType::PRIMITIVE_CREATION:{
            const PrimitiveCreationCommand& primitiveCreationCommand =
                    dynamic_cast< const PrimitiveCreationCommand& >( command );

            PrimitiveInfo primitiveInfo = primitiveCreationCommand.getPrimitiveInfo();
            std::string dstFilePath =
                    getCategoryAbsoluteePath( primitiveInfo.category ) +
                    "/" +
                    primitiveInfo.name +
                    boost::filesystem::extension( primitiveInfo.filePath );
            primitiveInfo.move( dstFilePath );

            log_->debug( "Primitive file received: [", primitiveInfo.filePath, "]\n" );

            registerPrimitive( primitiveCreationCommand.getPrimitiveID(),
                               primitiveInfo );

            // Emit a signal indicating the primitive insertion. Include
            // primitive's name and ID in the signal.
            emit primitiveAdded( primitiveCreationCommand.getPrimitiveID(),
                                 getPrimitiveRelativePath( primitiveCreationCommand.getPrimitiveID() ) );
        }break;
        case PrimitiveCommandType::PRIMITIVE_INSTANTIATION:{
            const PrimitiveInstantiationCommand& primitiveCommand =
                    dynamic_cast< const PrimitiveInstantiationCommand& >( command );

            instantiatePrimitive( primitiveCommand.getUserID(),
                                  primitiveCommand.getPrimitiveID(),
                                  primitiveCommand.getMeshID(),
                                  primitiveCommand.getMaterialID() );
        }break;
    }
}

} // namespace como
