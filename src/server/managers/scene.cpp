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

#include "scene.hpp"

namespace como {

const std::string SAVED_SCENES_DIR_PATH = "./data/save";

/***
 * 1. Construction
 ***/

Scene::Scene( const std::string& sceneName, CommandsHistoricPtr commandsHistoric, UsersMap& users, ResourceIDsGeneratorPtr resourceIDsGenerator, LogPtr log, const std::string& sceneFilePath ) :
    BasicScene( sceneName, log ),
    resourceIDsGenerator_( resourceIDsGenerator ),
    resourcesSyncLibrary_( commandsHistoric,
                           users,
                           getDirPath(),
                           getTempDirPath(),
                           resourceIDsGenerator_,
                           log ),
    nextUserID_( 1 )
{
    boost::filesystem::create_directories( SAVED_SCENES_DIR_PATH );

    if( sceneFilePath != "" ){
        loadFromFile( sceneFilePath );
    }else{
        initEmptyScene();
    }
}


/***
 * 2. Destruction
 ***/

Scene::~Scene()
{
    // TODO: Remove
    saveToFile( "autosave", true );
}


/***
 * 3. Command processing
 ***/

void Scene::processCommand( const Command& command )
{
    lock();
    resourcesSyncLibrary_.processCommand( command );
}


/***
 * 4. Scene saving / loading
 ***/

void Scene::saveToFile( const std::string &fileName, bool replace )
{
    lock();
    char buffer[8];
    std::string filePath = SAVED_SCENES_DIR_PATH + '/' + fileName;

    if( !replace && boost::filesystem::exists( filePath ) ){
        throw std::runtime_error(
                    std::string( "Scene::saveToFile - file already exists (" ) +
                    std::string( filePath ) +
                    std::string( ")" ) );
    }

    std::ofstream file( filePath, std::ios_base::binary );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    // Save the "next user's ID" to the file.
    const PackableUserID nextUserID( nextUserID_ );
    nextUserID.pack( buffer );
    file.write( buffer, nextUserID.getPacketSize() );

    // Save scene's resources to file.
    resourcesSyncLibrary_.saveToFile( file );

    file.close();
}


void Scene::loadFromFile( const std::string &filePath )
{
    char buffer[8];
    lock();
    std::ifstream file( filePath, std::ios_base::binary );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    // Load the "next user's ID" from the file.
    PackableUserID nextUserID;
    file.read( buffer, nextUserID.getPacketSize() );
    nextUserID.unpack( buffer );
    nextUserID_ = nextUserID.getValue();

    resourcesSyncLibrary_.readFromFile( file );

    file.close();
}


/***
 * 5. Getters
 ***/

UserID Scene::generateUserID()
{
    lock();
    return nextUserID_++;
}


/***
 * 6. Users management
 ***/

void Scene::removeUser( UserID userID )
{
    resourcesSyncLibrary_.removeUser( userID );
}


/***
 * 8. Initialization
 ***/

void Scene::initEmptyScene()
{
    const ResourceID DIRECTIONAL_LIGHT_ID = resourceIDsGenerator_->generateResourceIDs( 1 );
    const ResourceID CAMERA_ID = resourceIDsGenerator_->generateResourceIDs( 1 );

    // Create a directional light with with no owner and synchronize it in
    // the commands historic.
    std::uint8_t lightColor[4] = { 255, 255, 255, 255 };
    processCommand(
                DirectionalLightCreationCommand( NO_USER,
                                                 DIRECTIONAL_LIGHT_ID,
                                                 lightColor ) );

    // Create a camera with no owner and syncrhonize it in the commands
    // historic.
    const glm::vec3 cameraCenter( 0.0f, 0.0f, 0.0f );
    const glm::vec3 cameraEye( 0.5f, 0.5f, 0.0f );
    const glm::vec3 cameraUp( -0.5f, 0.5f, 0.0f );
    processCommand(
                CameraCreationCommand(
                    CAMERA_ID,
                    cameraCenter,
                    cameraEye,
                    cameraUp ) );
}


} // namespace como
