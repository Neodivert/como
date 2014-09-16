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

#include "abstract_textures_manager.hpp"
#include <boost/filesystem.hpp>

namespace como {


/***
 * 1. Construction
 ***/

AbstractTexturesManager::AbstractTexturesManager( const std::string &scenesDirPath ) :
    TEXTURES_DIR_PATH_( scenesDirPath + "/textures" )
{
    boost::system::error_code errorCode;
    boost::filesystem::create_directory( TEXTURES_DIR_PATH_, errorCode );

    if( errorCode ){
        throw std::runtime_error( "Couldn't create textures dir path [" +
                                  TEXTURES_DIR_PATH_ +
                                  "] - " +
                                  errorCode.message() );
    }
}


/***
 * 2. Destruction
 ***/

AbstractTexturesManager::~AbstractTexturesManager()
{
    boost::filesystem::remove_all( TEXTURES_DIR_PATH_ );
}

} // namespace como
