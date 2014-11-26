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
#include <iostream>

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
        log_->debug( "Loading scene from file [",
                      sceneFilePath,
                      "]\n" );
        loadFromFile( sceneFilePath );
    }else{
        log_->debug( "Initializing an empty scene\n" );
        initEmptyScene();
    }
}


/***
 * 2. Destruction
 ***/

Scene::~Scene()
{
    saveToFile( generateSaveFilePath( "autosave" ) );
}


/***
 * 3. Command processing
 ***/

void Scene::processCommand( const Command& command )
{
    LOCK
    resourcesSyncLibrary_.processCommand( command );
}


/***
 * 4. Scene saving / loading
 ***/

std::string Scene::saveToFile()
{
    LOCK
    bool saveToFile = false;
    bool exitLoop = false;
    std::string fileName;
    std::string filePath;

    while( !exitLoop ){
        std::cout << std::endl << std::endl;

        fileName = askForUserString( "Enter a file name for saving this scene into (Press ENTER if you don't want to save the scene): " );

        if( fileName.size() == 0 &&
                askForUserResponse( "Are you sure that you don't want to save scene? (y/n): " ) ){
            return "";
        }

        if( fileName.size() > 0 ){
            filePath = generateSaveFilePath( fileName );

            if( boost::filesystem::exists( filePath ) ){
                saveToFile =
                        exitLoop =
                            askForUserResponse( "File already exists. Do you want to overwrite it? (y/n): " );
            }else{
                saveToFile = exitLoop = true;
            }
        }
    }

    if( !saveToFile ){
        return "";
    }

    this->saveToFile( filePath );

    return filePath;
}


void Scene::loadFromFile( const std::string &filePath )
{
    char buffer[8];
    LOCK
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
    LOCK
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
 * 7. Auxiliar I/O methods
 ***/

bool Scene::askForUserResponse( const std::string& question )
{
    char userResponse = 'x';

    do {
        std::cout << question;
        std::cin >> userResponse;
        std::cin.get();
    }while( !std::cin.fail() && userResponse != 'n' && userResponse != 'y' );

    return ( userResponse == 'y' );
}


std::string Scene::askForUserString( const std::string &question )
{
    std::string userString;

    std::cout << question;
    std::getline( std::cin, userString );

    return userString;
}


void Scene::askForUserKeyPress( const std::string& message )
{
    std::cout << message;
    std::cin.get();
}


/***
 * 10. Initialization
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
    const glm::vec3 cameraEye( 0.5f, 0.5f, 0.5f );
    const glm::vec3 cameraUp( -0.5f, 0.5f, -0.5f );
    processCommand(
                CameraCreationCommand(
                    CAMERA_ID,
                    cameraCenter,
                    cameraEye,
                    cameraUp ) );
}


/***
 * 9. Auxiliar methods
 ***/

std::string Scene::generateSaveFilePath( const std::string &fileName )
{
#ifdef _WIN32
    const char DIR_SEPARATOR_CHAR = '\\';
#else
    const char DIR_SEPARATOR_CHAR = '/';
#endif
    return
        std::string( SAVED_SCENES_DIR_PATH ) +
        DIR_SEPARATOR_CHAR +
        fileName +
        std::string( ".csf" );
}


void Scene::saveToFile( const std::string &filePath )
{
    char buffer[8];

    // Open the file.
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

} // namespace como
