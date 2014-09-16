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

#include "texture_info.hpp"


namespace como {


/***
 * 1. Construction
 ***/

TextureInfo::TextureInfo(const std::string &filePath)
{
    loadFromFile( filePath );
}


TextureInfo::TextureInfo( std::ifstream &file, unsigned int nBytes )
{
    loadFromFile( file, nBytes );
}


/***
 * 2. File loading
 ***/

void TextureInfo::loadFromFile( const std::string& filePath )
{
    std::ifstream file;
    unsigned int imageFileSize = 0;

    if( !supportedImageFile( filePath ) ){
        throw std::runtime_error( "Texture image extension not supported (" +
                                  filePath +
                                  ")" );
    }

    file.open( filePath, std::ios_base::binary );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    // Retrieve the size of the texture image file.
    file.seekg( 0, file.end );
    imageFileSize = file.tellg();
    file.seekg( 0, file.beg );

    // Read texture data.
    loadFromFile( file, imageFileSize );

    file.close();
}


void TextureInfo::loadFromFile( std::ifstream &file, unsigned int nBytes )
{
    imageFileData.resize( nBytes );

    // Retrieve the size of the texture image file.
    file.read( &( imageFileData[0] ), nBytes );
}


/***
 * 3. Getters
 ***/

bool TextureInfo::supportedImageFile( const std::string& filePath )
{
    // List of supported image formats
    // (http://jcatki.no-ip.org:8080/SDL_image/SDL_image.html)
    // ".xcf" is not listed here although SDL_image supports it, because of the
    // following advice on SDL_image website: "This format is always changing,
    // and since there's no library supplied by the GIMP project to load XCF,
    // the loader may frequently fail to load much of any image from an XCF
    // file. It's better to load this in GIMP and convert to a better supported
    // image format."
    std::array< std::string, 15 > supportedExtensions =
    {
        ".tga",
        ".bmp",
        ".pnm",
        ".pgm",
        ".ppm",
        ".xpm",
        ".pcx",
        ".gif",
        ".jpg",
        ".jpeg",
        ".tif",
        ".tiff",
        ".lbm",
        ".iff",
        ".png"
    };

    std::string fileExtension = boost::filesystem::extension( filePath );

    for( const auto& supportedExtension : supportedExtensions ){
        if( fileExtension == supportedExtension ){
            return true;
        }
    }
    return false;
}


} // namespace como
