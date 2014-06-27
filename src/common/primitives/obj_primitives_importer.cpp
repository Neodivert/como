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

#include "obj_primitives_importer.hpp"
#include <fstream>
#include <stdexcept>
#include <common/exceptions/file_not_open_exception.hpp>

namespace como {

/***
 * 3. Primitives import
 ***/

PrimitiveInfo OBJPrimitivesImporter::importPrimitive( std::string primitiveName, std::string srcFilePath, std::string dstDirectory )
{
    std::ifstream srcFile;
    std::ofstream dstFile;
    std::string fileLine;
    std::string dstFilePath;
    std::string srcMaterialFilePath;
    std::string srcMaterialFileName;
    PrimitiveInfo primitive;
    bool materialProcessed = false;

    // Retrieve primitive name and mesh file.
    primitive.name = primitiveName;
    primitive.directory = dstDirectory;
    primitive.meshFileName =
            boost::filesystem::basename( srcFilePath ) +
            boost::filesystem::extension( srcFilePath );

    srcFile.open( srcFilePath );
    if( !srcFile.is_open() ){
        throw FileNotOpenException( srcFilePath );
    }

    dstFilePath = dstDirectory + '/' + primitiveName + ".obj";
    std::cout << "dstFilePath: [" << dstFilePath << "]" << std::endl;
    dstFile.open( dstFilePath.c_str() );
    std::cout << dstFile.is_open() << std::endl;
    if( !dstFile.is_open() ){
        srcFile.close();
        throw FileNotOpenException( dstFilePath );
    }

    // This loop copies the primitive source OBJ file replacing
    // "mtllib <material>" line with the new material name.
    // We also import the referenced material file.
    while( !srcFile.eof() ){
        std::getline( srcFile, fileLine );

        if( fileLine.size() ){
            if( fileLine.substr( 0, strlen( "mtllib" ) ) == "mtllib" ){
                if( materialProcessed ){
                    srcFile.close();
                    dstFile.close();
                    throw std::runtime_error( "Only one .mtl file per primitive allowed" );
                }

                // Retrieve the material file path.
                srcMaterialFileName = fileLine.substr( strlen( "mtllib" ) + 1 );
                srcMaterialFilePath = srcFilePath.substr( 0, srcFilePath.rfind( '/' ) + 1 ) + srcMaterialFileName;

                // Import the material file path.
                importMaterialFile( primitive, srcMaterialFilePath, dstDirectory );

                dstFile << "mtllib " << primitive.materialFileName << std::endl;
                materialProcessed = true;
            }else{
                dstFile << fileLine << std::endl;
            }
        }
    }

    srcFile.close();
    dstFile.close();

    if( !materialProcessed ){
        throw std::runtime_error( "A primitive can't be define without a material" );
    }

    return primitive;
}


void OBJPrimitivesImporter::importMaterialFile( PrimitiveInfo& primitive, std::string srcFilePath, std::string dstDirectory )
{
    std::string dstMaterialFilePath;
    std::ifstream srcFile;
    std::ofstream dstFile;
    std::string fileLine;
    std::string srcTextureFileName;
    std::string srcTextureFilePath;

    // Generate the material file name.
    primitive.materialFileName = primitive.name + boost::filesystem::extension( srcFilePath );

    // Generate the material destination dir path.
    dstMaterialFilePath = dstDirectory + '/' + primitive.materialFileName;

    srcFile.open( srcFilePath );
    if( !srcFile.is_open() ){
        throw FileNotOpenException( srcFilePath );
    }

    dstFile.open( dstMaterialFilePath );
    if( !dstFile.is_open() ){
        srcFile.close();
        throw FileNotOpenException( dstMaterialFilePath );
    }

    while( !srcFile.eof() ){
        std::getline( srcFile, fileLine );

        if( fileLine.size() ){
            if( fileLine.substr( 0, strlen( "map_Kd" ) ) == "map_Kd" ){
                // Retrieve the texture file path.
                srcTextureFileName = fileLine.substr( strlen( "mtllib" ) + 1 );
                srcTextureFilePath = srcFilePath.substr( 0, srcFilePath.rfind( '/' ) + 1 ) + srcTextureFileName;

                // Import the texture file path.
                importTextureFile( primitive, srcTextureFilePath, dstDirectory );

                dstFile << "map_Kd " << primitive.textureFileName << std::endl;
            }else{
                dstFile << fileLine << std::endl;
            }
        }
    }

    srcFile.close();
    dstFile.close();
}


void OBJPrimitivesImporter::importTextureFile( PrimitiveInfo &primitive, std::string srcFilePath, std::string dstDirectory )
{
    boost::system::error_code errorCode;
    std::string dstTextureFilePath;

    // Generate the texture file name.
    primitive.textureFileName = primitive.name + boost::filesystem::extension( srcFilePath );

    // Generate the texture destination dir path.
    dstTextureFilePath = dstDirectory + '/' + primitive.textureFileName;

    // Copy the texture file to its final location.
    boost::filesystem::copy( srcFilePath, dstTextureFilePath, errorCode );

    std::cout << "Importing texture file: " << srcFilePath << ", " << dstTextureFilePath << std::endl;

    if( errorCode ){
        throw std::runtime_error( std::string( "ERROR importing texture file [" ) +
                                  srcFilePath +
                                  "] to [" +
                                  dstTextureFilePath +
                                  "]: " + errorCode.message() );
    }
}

} // namespace como
