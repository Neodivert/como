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

Scene::Scene( const std::string& sceneName, CommandsHistoricPtr commandsHistoric, LogPtr log, const std::string& sceneFilePath ) :
    BasicScene( sceneName, log ),
    resourcesSyncLibrary_( commandsHistoric, getTempDirPath() )
{
    boost::filesystem::create_directories( SAVED_SCENES_DIR_PATH );

    if( sceneFilePath != "" ){
        loadFromFile( sceneFilePath );
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
    resourcesSyncLibrary_.processCommand( command );
}


/***
 * 4. Scene saving / loading
 ***/

void Scene::saveToFile( const std::string &fileName, bool replace )
{
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

    resourcesSyncLibrary_.saveToFile( file );

    file.close();
}


void Scene::loadFromFile( const std::string &filePath )
{
    std::ifstream file( filePath, std::ios_base::binary );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    resourcesSyncLibrary_.readFromFile( file );

    file.close();
}


} // namespace como