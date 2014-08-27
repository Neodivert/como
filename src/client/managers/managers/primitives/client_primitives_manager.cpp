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

ClientPrimitivesManager::ClientPrimitivesManager( std::string sceneDirPath, std::string sceneTempDirPath, ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, LogPtr log ) :
    AbstractPrimitivesManager( sceneDirPath, sceneTempDirPath, log ),
    server_( server ),
    meshesManager_( meshesManager ),
    materialsManager_( materialsManager )
{}


/***
 * 3. Primitives management
 ***/

std::string ClientPrimitivesManager::createPrimitive( std::string filePath, ResourceID categoryID )
{
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
    OBJPrimitivesImporter primitivesImporter;
    PrimitiveInfo primitive;
    char primitiveNameSuffix[64];

    // Generate the primitive name.
    ResourceID primitiveID = server_->getNewResourceID();
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
    MeshInfo meshInfo;
    ResourceID firstMaterialID;
    ResourceID meshID;

    PrimitiveFile::read( meshInfo, getPrimitiveFilePath( primitiveID ) );

    meshID = server_->getNewResourceID();

    materialsManager_->createMaterials( meshID, meshInfo.materialsData, firstMaterialID );

    meshesManager_->createMesh( meshID, meshInfo.vertexData, meshInfo.oglData, meshInfo.polygonGroupsData, materialsManager_->getMaterials( firstMaterialID, meshInfo.materialsData.size() ) );

    log_->debug( "Creating local mesh - Entity ID (", meshID,
                 ") First materialID ", firstMaterialID, ") nMaterials (", meshInfo.materialsData.size(), ")\n" );

    // Send the command to the server (the MaterialCreationCommand command was
    // already sent in previous call to materialsManager_->createMaterial() ).
    server_->sendCommand( CommandConstPtr( new PrimitiveInstantiationCommand( server_->getLocalUserID(), primitiveID, meshID, firstMaterialID ) ) );
}


// FIXME: Duplicated code.
void ClientPrimitivesManager::instantiatePrimitive( UserID userID, ResourceID primitiveID, ResourceID meshID, ResourceID materialID )
{
    (void)( userID );

    MeshInfo meshInfo;

    PrimitiveFile::read( meshInfo, getPrimitiveFilePath( primitiveID ) );

    materialsManager_->createMaterials( meshID, meshInfo.materialsData, materialID );
    meshesManager_->createMesh( meshID, meshInfo.vertexData, meshInfo.oglData, meshInfo.polygonGroupsData, materialsManager_->getMaterials( materialID, meshInfo.materialsData.size() ) );
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

            PrimitiveInfo primitiveInfo = primitiveCreationCommand->getPrimitiveInfo();
            std::string dstFilePath =
                    getCategoryAbsoluteePath( primitiveInfo.category ) +
                    "/" +
                    primitiveInfo.name +
                    boost::filesystem::extension( primitiveInfo.filePath );
            primitiveInfo.move( dstFilePath );

            log_->debug( "Primitive file received: [", primitiveInfo.filePath, "]\n" );

            registerPrimitive( primitiveCreationCommand->getPrimitiveID(),
                               primitiveInfo );

            // Emit a signal indicating the primitive insertion. Include
            // primitive's name and ID in the signal.
            // TODO: Complete
            emit primitiveAdded( primitiveCreationCommand->getPrimitiveID(),
                                 getPrimitiveRelativePath( primitiveCreationCommand->getPrimitiveID() ) );
        }break;
        case PrimitiveCommandType::PRIMITIVE_INSTANTIATION:{
            const PrimitiveInstantiationCommand* primitiveCommand =
                    dynamic_cast< const PrimitiveInstantiationCommand* >( command.get() );

            instantiatePrimitive( primitiveCommand->getUserID(),
                                  primitiveCommand->getPrimitiveID(),
                                  primitiveCommand->getMeshID(),
                                  primitiveCommand->getMaterialID() );
        }break;
    }
}

} // namespace como
