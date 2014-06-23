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

std::string ClientPrimitivesManager::createPrimitive( std::string filePath, ResourceID categoryID )
{
    char resourceIDString[20];

    ResourceID id = server_->getNewResourceID();

    sprintf( resourceIDString, "_%u_%u", id.getCreatorID(), id.getResourceIndex() );

    std::string primitiveName =
            boost::filesystem::basename( filePath ) +
            std::string( resourceIDString );

    std::string meshFileName =
            primitiveName +
            boost::filesystem::extension( filePath );

    registerPrimitive( id, meshFileName, categoryID );

    importMeshFile( primitiveName, filePath, getPrimitiveAbsolutePath( id ) );

    server_->sendCommand(
                CommandConstPtr(
                    new PrimitiveCreationCommand( getPrimitiveAbsolutePath( id ).c_str(),
                                                  id.getCreatorID(),
                                                  id,
                                                  categoryID ) ) );

    emit primitiveAdded( id, getPrimitiveRelativePath( id ) );

    return getPrimitiveRelativePath( id );
}


void ClientPrimitivesManager::importMeshFile( std::string primitiveName, std::string oldFilePath, std::string newFilePath )
{
    const unsigned int LINE_SIZE = 250;
    char line[LINE_SIZE] = {0};
    bool processMaterial = false;

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
        throw std::runtime_error( std::string( "Couldn't open file [" +
                                               oldFilePath + "]" ) );
    }

    // Create the new mesh file.
    std::ofstream newFile( newFilePath );

    if( !newFile.is_open() ){
        throw std::runtime_error( std::string( "Couldn't open file [" +
                                               oldFilePath + "]" ) );
    }

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

    // Copy the material name to the primitives directory.
    std::string oldMaterialFilePath = oldFilePath.substr( 0, oldFilePath.size() - boost::filesystem::extension( oldFilePath ).size() ) + ".mtl";
    std::string newMaterialFilePath = newFilePath.substr( 0, newFilePath.size() - boost::filesystem::extension( newFilePath ).size() ) + ".mtl";

    if( processMaterial ){
        log_->debug( "processMaterial: ", oldMaterialFilePath, ", ", newMaterialFilePath, "\n" );
        boost::filesystem::copy( oldMaterialFilePath, newMaterialFilePath );
    }
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
