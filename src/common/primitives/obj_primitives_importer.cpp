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

void OBJPrimitivesImporter::importPrimitive( std::string primitiveName, std::string srcFilePath, std::string dstDirectory )
{
    std::ifstream srcFile;
    std::ofstream dstFile;
    std::string fileLine;
    std::string dstFilePath;
    std::string srcMaterialFilePath;
    std::string srcMaterialFileName;
    std::string dstMaterialFileName;

    srcFile.open( srcFilePath );
    if( !srcFile.is_open() ){
        throw FileNotOpenException( srcFilePath );
    }


    dstFilePath = dstDirectory + '/' + primitiveName + ".obj";
    std::cout << "dstFilePath: [" << dstFilePath << "]" << std::endl;
    dstFile.open( dstFilePath.c_str() );
    std::cout << dstFile.is_open() << std::endl;
    if( !dstFile.is_open() ){
        throw std::runtime_error( strerror(errno) );
        throw FileNotOpenException( dstFilePath );
    }

    // This loop copies the primitive source OBJ file replacing
    // "mtllib <material>" line with the new material name.
    // We also import the referenced material file.
    while( !srcFile.eof() ){
        std::getline( srcFile, fileLine );

        if( fileLine.size() ){
            if( fileLine.substr( 0, strlen( "mtllib" ) ) == "mtllib" ){
                // Retrieve the material file name and retrieve the new name.
                srcMaterialFileName = fileLine.substr( strlen( "mtllib" ) + 1 );
                dstMaterialFileName = primitiveName + '_' + dstMaterialFileName;

                dstFile << "mtllib " << dstMaterialFileName << std::endl;

                srcMaterialFilePath = srcFilePath.substr( 0, srcFilePath.rfind( '/' ) + 1 ) + srcMaterialFileName;

                importMaterialFile( primitiveName, srcMaterialFilePath, dstDirectory );
            }else{
                dstFile << fileLine << std::endl;
            }
        }
    }

    srcFile.close();
    dstFile.close();
}


void OBJPrimitivesImporter::importMaterialFile( std::string primitiveName, std::string srcFilePath, std::string dstDirectory )
{
    boost::filesystem::copy( srcFilePath, dstDirectory + '/' + primitiveName + ".mtl" );
}

} // namespace como
