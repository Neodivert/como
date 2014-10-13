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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "server_primitives_manager.hpp"
#include <common/primitives/obj_primitives_importer.hpp>

namespace como {

/***
 * 1. Construction
 ***/

ServerPrimitivesManager::ServerPrimitivesManager(const std::string& sceneDirPath,
                                                  const std::string& tempDirPath,
                                                  CommandsHistoricPtr commandsHistoric,
                                                  LogPtr log,
                                                  ResourceIDsGeneratorPtr resourceIDsGenerator ) :
    AbstractPrimitivesManager( sceneDirPath, tempDirPath, log ),
    commandsHistoric_( commandsHistoric ),
    resourceIDsGenerator_( resourceIDsGenerator )
{
    // Sync server's local primitives directory.
    syncPrimitivesDir();

    createCategory( "Uncategorized" );
}


/***
 * 2. Initialization
 ***/

void ServerPrimitivesManager::createPrimitivesDir()
{
    lock();
    char consoleCommand[256] = {0};
    int lastCommandResult = 0;

    log_->debug( "Populating scene primitives directory [", scenePrimitivesDir_, "] ...\n" );

    // Copy the server's local directory to this scene's directory.
    // TODO: Use a multiplatform alternative (boost::filesystem::copy_directory
    // doesn't copy directory's contents).
    sprintf( consoleCommand, "cp -RT \"%s\"* \"%s\"", LOCAL_PRIMITIVES_DIR, scenePrimitivesDir_.c_str() );
    log_->debug( consoleCommand, "\n" );
    lastCommandResult = system( consoleCommand );

    // If there was any error creating the scene primitives directory, throw
    // an exception.
    if( lastCommandResult ){
        throw std::runtime_error( std::string( "Error copying contents to scene primitives directory [" ) +
                                  scenePrimitivesDir_ +
                                  "]"
                                  );
    }

    log_->debug( "Populating scene primitives directory [", scenePrimitivesDir_, "] ...OK\n" );
}


void ServerPrimitivesManager::syncPrimitivesDir()
{
    lock();
    //createPrimitivesDir();

    const char* filePath = nullptr;
    const boost::filesystem::directory_iterator endIterator;
    boost::filesystem::directory_iterator fileIterator( LOCAL_PRIMITIVES_DIR );

    log_->debug( "Adding primitives to scene [", LOCAL_PRIMITIVES_DIR, "] ...\n" );

    for( ; fileIterator != endIterator; fileIterator++ ){
        if( boost::filesystem::is_directory( *fileIterator ) ){
            filePath = fileIterator->path().string().c_str();

            syncPrimitivesCategoryDir( filePath );
        }
    }

    log_->debug( "Adding primitives to scene [", LOCAL_PRIMITIVES_DIR, "] ...OK\n" );
}


ResourceID ServerPrimitivesManager::createCategory( std::string name )
{
    lock();

    const ResourceID categoryID = resourceIDsGenerator_->generateResourceIDs( 1 );

    AbstractPrimitivesManager::createCategory( categoryID, name );

    commandsHistoric_->addCommand( CommandConstPtr(
                                       new PrimitiveCategoryCreationCommand( 0,
                                                                             categoryID,
                                                                             name ) ) );

    return categoryID;
}


void ServerPrimitivesManager::syncPrimitivesCategoryDir( std::string dirPath )
{
    lock();

    const boost::filesystem::directory_iterator endIterator;
    boost::filesystem::directory_iterator fileIterator( dirPath );
    std::string filePath;
    OBJPrimitivesImporter primitivesImporter;
    ResourceID categoryID;
    PrimitiveInfo primitive;
    char nameSuffix[30] = {0};

    log_->debug( "Synchronizing category dir [", dirPath, "]\n" );

    categoryID = createCategory( boost::filesystem::basename( dirPath ) );

    boost::filesystem::create_directory( getCategoryAbsoluteePath( categoryID /*primitive.category*/ ) );

    for( ; fileIterator != endIterator; fileIterator++ ){
        if( boost::filesystem::is_regular_file( *fileIterator ) ){
            filePath = fileIterator->path().string();

            log_->debug( "filePath: ", filePath, "\n" );

            if( boost::filesystem::extension( filePath ) == ".obj" ){
                try {
                    ResourceID primitiveID = resourceIDsGenerator_->generateResourceIDs( 1 );

                    sprintf( nameSuffix, "__%i_%i", primitiveID.getCreatorID(), primitiveID.getResourceIndex() );

                    primitive = primitivesImporter.importPrimitive( filePath,
                                                                    getCategoryAbsoluteePath( categoryID ),
                                                                    nameSuffix );
                    primitive.category = categoryID;

                    registerPrimitive( primitive, primitiveID );
                }catch( std::exception& ex ){
                    log_->error( "\n\nError importing primitive [",
                                 filePath, "] - ",
                                 ex.what(), "\n\n" );
                }
            }
        }
    }
}


/***
 * 4. Categories management
 ***/

ResourceID ServerPrimitivesManager::registerCategory( std::string categoryName )
{
    lock();

    const ResourceID categoryID = resourceIDsGenerator_->generateResourceIDs( 1 );

    // Register the the given category.
    AbstractPrimitivesManager::registerCategory( categoryID, categoryName );

    // Add the appropriate category creation command to the commands historic.
    log_->debug( "\tAdding primitive category [", categoryName, "] to scene ...\n" );
    commandsHistoric_->addCommand( CommandConstPtr( new PrimitiveCategoryCreationCommand( 0, categoryID, categoryName  ) ) );
    log_->debug( "\tAdding primitive category [", categoryName, "] to scene ...OK\n" );

    return categoryID;
}


/***
 * 5. Primitives management
 ***/

void ServerPrimitivesManager::registerPrimitive( PrimitiveInfo primitive )
{
    lock();

    const ResourceID primitiveID = resourceIDsGenerator_->generateResourceIDs( 1 );
    registerPrimitive( primitive, primitiveID );
}


void ServerPrimitivesManager::registerPrimitive(PrimitiveInfo primitive, const ResourceID &primitiveID)
{
    lock();
    // We are about to create a command which needs to keep a copy of the
    // current primitive, so we create such copy in the tmp directory.
    PrimitiveInfo primitiveCopy = primitive.copy(
                tempDirPath_ + '/' +
                primitive.name + "_" +
                getCurrentDateTimeStr() +
                boost::filesystem::extension( primitive.filePath ) );
    log_->debug( "Primitive creation command created for primitive (",
                 primitiveCopy.filePath, ")\n" );

    AbstractPrimitivesManager::registerPrimitive( primitiveID, primitive );

    commandsHistoric_->addCommand( CommandConstPtr( new PrimitiveCreationCommand( 0,
                                                                                  primitiveID,
                                                                                  primitiveCopy,
                                                                                  tempDirPath_ ) ) );
}

} // namespace como
