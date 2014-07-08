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

ClientPrimitivesManager::ClientPrimitivesManager( std::string sceneDirPath, std::string sceneTempDirPath, ServerInterfacePtr server, DrawablesManagerPtr drawablesManager, MaterialsManagerPtr materialsManager, LogPtr log ) :
    AbstractPrimitivesManager( sceneDirPath, sceneTempDirPath, log ),
    server_( server ),
    drawablesManager_( drawablesManager ),
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

    emit primitiveAdded( id, getPrimitiveFilePath( id ) );

    return getPrimitiveFilePath( id );
}


ResourceID ClientPrimitivesManager::importMeshFile( std::string srcFilePath, ResourceID categoryID )
{
    OBJPrimitivesImporter primitivesImporter;
    PrimitiveInfo primitive;
    char primitiveName[64];

    // Generate the primitive name.
    ResourceID primitiveID = server_->getNewResourceID();
    sprintf( primitiveName, "%s_%u_%u",
             boost::filesystem::basename( srcFilePath ).c_str(),
             primitiveID.getCreatorID(),
             primitiveID.getResourceIndex() );

    primitive = primitivesImporter.importPrimitive( primitiveName, srcFilePath, getCategoryAbsoluteePath( categoryID ) );
    primitive.category = categoryID;
    registerPrimitive( primitiveID, primitive );

    return primitiveID;
}

void ClientPrimitivesManager::instantiatePrimitive( ResourceID primitiveID )
{
    MeshInfo meshInfo;

    PrimitiveFile::read( meshInfo, getPrimitiveFilePath( primitiveID ) );

    MaterialID materialID = materialsManager_->createMaterial( meshInfo.materialsData[0] ); // TODO: Use multiple materials?

    PackableDrawableID drawableID = drawablesManager_->createMesh( meshInfo.vertexData, meshInfo.oglData, materialsManager_->getMaterial( materialID ) );

    log_->debug( "Creating local mesh - Drawable ID (", drawableID,
                 ") MaterialID ", materialID, "\n" );

    // Send the command to the server (the MaterialCreationCommand command was
    // already sent in previous call to materialsManager_->createMaterial() ).
    server_->sendCommand( CommandConstPtr( new PrimitiveInstantiationCommand( server_->getLocalUserID(), primitiveID, drawableID, materialID ) ) );
}


// FIXME: Duplicated code.
void ClientPrimitivesManager::instantiatePrimitive( UserID userID, ResourceID primitiveID, PackableDrawableID meshID, MaterialID materialID )
{
    (void)( userID );

    MeshInfo meshInfo;

    PrimitiveFile::read( meshInfo, getPrimitiveFilePath( primitiveID ) );

    materialsManager_->createMaterial( materialID, meshInfo.materialsData[0] ); // TODO: Use multiple materials?

    drawablesManager_->createMesh( meshID, meshInfo.vertexData, meshInfo.oglData, materialsManager_->getMaterial( materialID ) );
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
                                 getPrimitiveFilePath( primitiveCreationCommand->getPrimitiveID() ) );
        }break;
        case PrimitiveCommandType::PRIMITIVE_INSTANTIATION:{
            const PrimitiveInstantiationCommand* primitiveCommand =
                    dynamic_cast< const PrimitiveInstantiationCommand* >( command.get() );

            PackableDrawableID meshID;
            meshID.creatorID = primitiveCommand->getMeshID().getCreatorID();
            meshID.drawableIndex = static_cast< DrawableIndex >( primitiveCommand->getMeshID().getResourceIndex() );

            instantiatePrimitive( primitiveCommand->getUserID(),
                                  primitiveCommand->getPrimitiveID(),
                                  meshID,
                                  MaterialID( primitiveCommand->getMaterialID().getCreatorID(), static_cast< MaterialIndex >( primitiveCommand->getMaterialID().getResourceIndex() ) ) );
        }break;
    }
}

} // namespace como
