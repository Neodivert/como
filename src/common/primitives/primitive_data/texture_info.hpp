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

#ifndef TEXTURE_INFO_HPP
#define TEXTURE_INFO_HPP

#include <string>
#include <fstream>
#include <boost/filesystem.hpp>
#include <common/exceptions/file_not_open_exception.hpp>

namespace como {

struct TextureInfo {
    std::string imageFileData;


    /***
     * 1. Construction
     ***/
    TextureInfo( const std::string& filePath );
    TextureInfo( std::ifstream& file, unsigned int nBytes );


    /***
     * 2. File loading
     ***/
    void loadFromFile( const std::string& filePath );
    void loadFromFile( std::ifstream& file, unsigned int nBytes );


    /***
     * 3. Getters
     ***/
    static bool supportedImageFile( const std::string& filePath );
};


}

#endif // TEXTURE_INFO_HPP
