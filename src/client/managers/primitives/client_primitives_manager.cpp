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

ClientPrimitivesManager::ClientPrimitivesManager( std::string sceneDirPath, ServerInterfacePtr server, DrawablesManagerPtr drawablesManager, MaterialsManagerPtr materialsManager, LogPtr log ) :
    AbstractPrimitivesManager( sceneDirPath, log ),
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
                                                  categoryID,
                                                  getPrimitiveAbsolutePath( id, PrimitiveComponent::MESH ).c_str() ,
                                                  getPrimitiveAbsolutePath( id, PrimitiveComponent::MATERIAL ).c_str() ) ) );

    emit primitiveAdded( id, getPrimitiveRelativePath( id, PrimitiveComponent::MESH ) );

    return getPrimitiveRelativePath( id, PrimitiveComponent::MESH );
}


ResourceID ClientPrimitivesManager::importMeshFile( std::string oldFilePath, ResourceID categoryID )
{
    const unsigned int LINE_SIZE = 250;
    char line[LINE_SIZE] = {0};
    bool processMaterial = false;
    char resourceIDString[20];

    ResourceID primitiveID = server_->getNewResourceID();
    sprintf( resourceIDString, "_%u_%u", primitiveID.getCreatorID(), primitiveID.getResourceIndex() );

    std::string primitiveName =
            boost::filesystem::basename( oldFilePath ) +
            std::string( resourceIDString );

    std::string meshFileName =
            primitiveName +
            boost::filesystem::extension( oldFilePath );

    std::string newFilePath = getCategoryAbsoluteePath( categoryID ) + '/' + meshFileName;

    log_->debug( "Importing mesh file (", primitiveName, ", ",
                 oldFilePath, ", ",
                 newFilePath, ")\n" );

    std::string materialName;

    // The material file name will be given a name equal to the mesh file but
    // with a .mtl extension.
    std::string newMaterialFileName = primitiveName + ".mtl";

    if( boost::filesystem::exists( newFilePath ) ){
        throw std::runtime_error( std::string( "Mesh file [" ) +
                                  newFilePath + "] already exists" );
    }

    // Open the original mesh file.
    std::ifstream oldFile( oldFilePath );

    if( !oldFile.is_open() ){
        throw std::runtime_error( std::string( "Importing mesh file (old) - Couldn't open file [" +
                                               oldFilePath + "]" ) );
    }

    // Create the new mesh file.
    std::ofstream newFile( newFilePath );

    if( !newFile.is_open() ){
        throw std::runtime_error( std::string( "Importing mesh file (new) - Couldn't open file [" +
                                               newFilePath + "]" ) );
    }

    log_->debug( "oldFilePath: ", oldFilePath, "\n" );
    log_->debug( "newFilePath: ", newFilePath, "\n" );

    // Start to copy the contents of the original mesh file to the new mesh
    // file in primitives directory. If a line including the material
    // name appears, replace such name by the new one.
    while( !oldFile.eof() ){
        oldFile.getline( line, LINE_SIZE );

        if( !strncmp( line, "mtllib", 6 ) ){
            processMaterial = true;

            materialName = std::string( line ).substr( 7 );

            log_->debug( "Material name: ", materialName, "\n" );

            newFile << "mtllib " << newMaterialFileName << std::endl;
        }else{
            newFile << line << std::endl;
        }
    }

    oldFile.close();
    newFile.close();

    if( !processMaterial ){
        boost::filesystem::remove( newFilePath );
        throw std::runtime_error( "Primitive must include a material" );
    }

    // Copy the material name to the primitives directory.
    std::string oldMaterialFilePath = oldFilePath.substr( 0, oldFilePath.size() - boost::filesystem::extension( oldFilePath ).size() ) + ".mtl";
    std::string newMaterialFilePath = newFilePath.substr( 0, newFilePath.size() - boost::filesystem::extension( newFilePath ).size() ) + ".mtl";

    log_->debug( "oldMaterialFilePath: ", oldMaterialFilePath, "\n" );
    log_->debug( "newMaterialFilePath: ", newMaterialFilePath, "\n" );

    log_->debug( "processMaterial: ", oldMaterialFilePath, ", ", newMaterialFilePath, "\n" );
    boost::filesystem::copy( oldMaterialFilePath, newMaterialFilePath );

    // Register the primitive into the system.
    registerPrimitive( primitiveID, categoryID, meshFileName, newMaterialFileName );

    return primitiveID;
}

void ClientPrimitivesManager::instantiatePrimitive( ResourceID primitiveID )
{
    // Get the "absolute" path to the specification file of the
    // primitive used for building this mesh.
    std::string primitivePath = getPrimitiveAbsolutePath( primitiveID, PrimitiveComponent::MESH );

    // Create the material.
    MaterialID materialID = materialsManager_->createMaterial( "Unnamed material" ); // TODO: Use another name.

    // Create the mesh.
    DrawablePtr drawable = DrawablePtr( new Mesh( primitivePath.c_str(), materialsManager_->getMaterial( materialID ) ) );
    PackableDrawableID drawableID = drawablesManager_->addDrawable( drawable );

    log_->debug( "Creating local mesh - Drawable ID (", drawableID,
                 ") MaterialID ", materialID, "\n" );

    // Send the command to the server (the MaterialCreationCommand command was
    // already sent in previous call to materialsManager_->createMaterial() ).
    server_->sendCommand( CommandConstPtr( new PrimitiveInstantiationCommand( primitiveID, drawableID, materialID ) ) );
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

            log_->debug( "Primitive file received: [", primitiveCreationCommand->getMeshFile()->getFilePath(), "]\n" );

            registerPrimitive( primitiveCreationCommand->getPrimitiveID(),
                               primitiveCreationCommand->getCategoryID(),
                               primitiveCreationCommand->getMeshFileName(),
                               primitiveCreationCommand->getMaterialFileName() );

            // Emit a signal indicating the primitive insertion. Include
            // primitive's name and ID in the signal.
            // TODO: Complete
            emit primitiveAdded( primitiveCreationCommand->getPrimitiveID(),
                                 getPrimitiveRelativePath( primitiveCreationCommand->getPrimitiveID(), PrimitiveComponent::MESH ) );
        }break;
        case PrimitiveCommandType::PRIMITIVE_INSTANTIATION:
            // TODO: Complete.
        break;
    }
}

} // namespace como
