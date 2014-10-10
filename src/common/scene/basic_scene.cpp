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

#include "basic_scene.hpp"

namespace como {


/***
 * 1. Construction
 ***/

BasicScene::BasicScene( LogPtr log ) :
    sceneName_( "" ),
    sceneDirPath_( "" ),
    sceneTempDirPath_( "" ),
    log_( log )
{}

BasicScene::BasicScene( const std::string& sceneName, LogPtr log ) :
    sceneName_( sceneName ),
    sceneDirPath_( "" ),
    sceneTempDirPath_( "" ),
    log_( ( log != nullptr ) ? log : nullptr )
{
    createSceneDirectory();
}


/***
 * 2. Destruction
 ***/

BasicScene::~BasicScene()
{
    boost::filesystem::remove_all( sceneDirPath_ );
    log_->debug( "Scene directory removed [", sceneDirPath_, "]\n" );
}


/***
 * 3. Initialization
 ***/

void BasicScene::createSceneDirectory()
{
    sceneDirPath_ = std::string( SCENES_DIR ) + '/' + sceneName_;

    unsigned int nameCounter = 1;
    while( boost::filesystem::exists( sceneDirPath_ ) ){
        sceneDirPath_ = std::string( SCENES_DIR ) + '/' + sceneName_ + '_' + std::to_string( nameCounter );
        nameCounter++;
    }
    sceneTempDirPath_ = sceneDirPath_ + "/.temp";

    boost::filesystem::create_directories( sceneDirPath_ );
    log_->debug( "Scene directory created [", sceneDirPath_, "]\n" );

    boost::filesystem::create_directory( sceneTempDirPath_ );
    log_->debug( "Scene temp directory created [", getTempDirPath(), "]\n" );
}


void BasicScene::initScene( const std::string &sceneName )
{
    sceneName_ = sceneName;

    createSceneDirectory();
}


/***
 * 4. Getters
 ***/

std::string BasicScene::getName() const
{
    return sceneName_;
}


std::string BasicScene::getDirPath() const
{
    return sceneDirPath_;
}


std::string BasicScene::getTempDirPath() const
{
    return sceneTempDirPath_;
}


LogPtr BasicScene::log() const
{
    return log_;
}


} // namespace como
