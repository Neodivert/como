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

#ifndef MATERIAL_INFO_HPP
#define MATERIAL_INFO_HPP

#include <string>
#include <glm/glm.hpp>
#include "texture_info.hpp"
#include <memory>
#include <vector>
#include <fstream>
#include <common/primitives/primitive_file.hpp> // TODO: Move PrimitiveFile::readVec3() method to another class.

namespace como {

struct MaterialInfo
{
    std::string name;

    glm::vec4 color;

    glm::vec3 ambientReflectivity;
    glm::vec3 diffuseReflectivity;
    glm::vec3 specularReflectivity;
    float specularExponent;

    std::unique_ptr< TextureInfo > textureInfo;

    MaterialInfo() : specularExponent( 1.0f ), textureInfo( nullptr ) {}
    inline MaterialInfo( const MaterialInfo& b );


    /***
     * 1. File reading
     ***/
    inline void readFromFile( std::ifstream& file );


    /***
     * 2. File writting
     ***/
    inline void writeToFile( std::ofstream& file ) const;
};

typedef std::vector< MaterialInfo > MaterialsInfoVector;


MaterialInfo::MaterialInfo( const MaterialInfo& b ) :
    name( b.name ),
    color( b.color ),
    ambientReflectivity( b.ambientReflectivity ),
    diffuseReflectivity( b.diffuseReflectivity ),
    specularReflectivity( b.specularReflectivity ),
    specularExponent( b.specularExponent )
{
    if( b.textureInfo ){
        textureInfo = std::unique_ptr< TextureInfo >( new TextureInfo( *( b.textureInfo ) ) );
    }
}


/***
 * 1. File reading
 ***/

void MaterialInfo::readFromFile( std::ifstream &file )
{
    std::string fileLine;

    // Read the material's name.
    std::getline( file, name );

    // Read the material's ambient reflectivity.
    std::getline( file, fileLine );
    ambientReflectivity = PrimitiveFile::readVec3( fileLine );

    // Read the material's diffuse reflectivity.
    std::getline( file, fileLine );
    diffuseReflectivity = PrimitiveFile::readVec3( fileLine );

    // Read the material's specular reflectivity.
    std::getline( file, fileLine );
    specularReflectivity = PrimitiveFile::readVec3( fileLine );

    // Read the material's specular exponent.
    std::getline( file, fileLine );
    specularExponent = atoi( fileLine.c_str() );

    // Next line is "1" if the current material contains a texture or "0"
    // otherwise.
    std::getline( file, fileLine );
    if( fileLine == "1" ){
        // Add an emtpy texture to the current
        textureInfo = std::unique_ptr< TextureInfo >( new TextureInfo );

        // Read texture data size.
        std::getline( file, fileLine );
        textureInfo->imageFileData.resize( atoi( fileLine.c_str() ) );

        // Read texture data.
        file.read( &( textureInfo->imageFileData[0] ),
                textureInfo->imageFileData.size() );

        // Remove the new line separator right after texture data.
        std::getline( file, fileLine );
    }
}


/***
 * 2. File writting
 ***/

void MaterialInfo::writeToFile( std::ofstream &file ) const
{
    file << name << std::endl;

    file << ambientReflectivity[0] << " "
         << ambientReflectivity[1] << " "
         << ambientReflectivity[2] << std::endl;

    file << diffuseReflectivity[0] << " "
         << diffuseReflectivity[1] << " "
         << diffuseReflectivity[2] << std::endl;

    file << specularReflectivity[0] << " "
         << specularReflectivity[1] << " "
         << specularReflectivity[2] << std::endl;

    file << specularExponent << std::endl;

    // If current material includes a texture, we write the following data to
    // file:
    // - A line "1" indicating that the current material includes a texture.
    // - A line "<n>" where n is the size (in bytes) of the texture image file
    // data.
    // - <n> bytes of the texture image file.
    if( textureInfo ){
        file << "1" << std::endl
             << textureInfo->imageFileData.size() << std::endl;

        // Write texture data.
        file.write( &( textureInfo->imageFileData[0] ),
                textureInfo->imageFileData.size() );

        // Add a new line right after texture data.
        file << std::endl;
    }else{
        file << "0" << std::endl;
    }
}


} // namespace como

#endif // MATERIAL_INFO_HPP
