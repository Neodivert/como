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

namespace como {

/***
 * 1. Construction
 ***/

ServerPrimitivesManager::ServerPrimitivesManager( std::string sceneName_, CommandsHistoricPtr commandsHistoric, LogPtr log ) :
    nextPrimitiveCategoryID_( 0 ),
    nextPrimitiveID_( 0 ),
    commandsHistoric_( commandsHistoric ),
    log_( log )
{
    // Build the path to the scene's primitives directory.
    scenePrimitivesDir_ = std::string( SCENES_DIR ) + "/" + sceneName_ + "/primitives";

    // Sync server's local primitives directory.
    syncPrimitivesDir();
}


/***
 * 2. Initialization
 ***/

void ServerPrimitivesManager::createPrimitivesDir()
{
    char consoleCommand[256] = {0};
    int lastCommandResult = 0;

    log_->debug( "Creating scene primitives directory [", scenePrimitivesDir_, "] ...\n" );

    // Create the scene primitives directory.
    sprintf( consoleCommand, "mkdir -p \"%s\"", scenePrimitivesDir_.c_str() );
    log_->debug( consoleCommand, "\n" );
    lastCommandResult = system( consoleCommand );
    if( lastCommandResult ){
        throw std::runtime_error( std::string( "Error creating scene primitives directory [" ) +
                                  scenePrimitivesDir_ +
                                  "]"
                                  );
    }

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

    log_->debug( "Creating scene primitives directory [", scenePrimitivesDir_, "] ...OK\n" );
}


void ServerPrimitivesManager::syncPrimitivesDir()
{
    const char* filePath = nullptr;
    const boost::filesystem::directory_iterator endIterator;
    boost::filesystem::directory_iterator fileIterator( scenePrimitivesDir_ );

    createPrimitivesDir();

    log_->debug( "Adding primitives to scene [", scenePrimitivesDir_, "] ...\n" );

    for( ; fileIterator != endIterator; fileIterator++ ){
        if( boost::filesystem::is_directory( *fileIterator ) ){
            filePath = fileIterator->path().string().c_str();

            syncPrimitivesCategoryDir( filePath );
        }
    }

    log_->debug( "Adding primitives to scene [", scenePrimitivesDir_, "] ...OK\n" );
}


void ServerPrimitivesManager::syncPrimitivesCategoryDir( std::string dirPath )
{
    addCategory( boost::filesystem::basename( dirPath ) );

    // use PrimitivesID
}


/***
 * 4. Categories management
 ***/

void ServerPrimitivesManager::addCategory( std::string categoryName )
{
    if( categoryNameInUse( categoryName ) ){
        throw std::runtime_error( std::string( "Category name [" ) +
                                  categoryName +
                                  "] already in use" );
    }

    log_->debug( "\tAdding primitive category [", categoryName, "] to scene ...\n" );
    commandsHistoric_->addCommand( CommandConstPtr( new PrimitiveCategoryCreationCommand( 0, nextPrimitiveCategoryID_, categoryName  ) ) );
    log_->debug( "\tAdding primitive category [", categoryName, "] to scene ...OK\n" );

    categoryNames_[ nextPrimitiveCategoryID_ ] = categoryName;
}


/***
 * 5. Auxiliar methods
 ***/

bool ServerPrimitivesManager::categoryNameInUse( std::string categoryName )
{
    for( auto categoryPair : categoryNames_ ){
        if( categoryPair.second == categoryName ){
            return true;
        }
    }
    return false;
}

} // namespace como
