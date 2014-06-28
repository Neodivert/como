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
#include <map>
#include <array>

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

    MeshInfo meshInfo;
    processMeshFile( srcFilePath, meshInfo );

    if( !materialProcessed ){
        throw std::runtime_error( "A primitive can't be define without a material" );
    }

    return primitive;
}


void OBJPrimitivesImporter::processMeshFile( std::string filePath, MeshInfo& meshInfo )
{
    std::ifstream file;
    std::string fileLine;

    file.open( filePath );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    while( !file.eof() ){
        std::getline( file, fileLine );

        processMeshFileLine( fileLine, meshInfo );
    }

    std::cout << "Mesh: " << std::endl
              << "-------------------------------------------" << std::endl;
    std::cout << "Vertices: " << std::endl;
    for( auto v : meshInfo.vertices ){
        std::cout << "\t(" << v[0] << ", " << v[1] << ", " << v[2] << ")" << std::endl;
    }
    std::cout << "UV coordinates: " << std::endl;
    for( auto v : meshInfo.uvCoordinates ){
        std::cout << "\t(" << v[0] << ", " << v[1] << ")" << std::endl;
    }
    std::cout << "Vertex triangles: " << std::endl;
    for( auto v : meshInfo.vertexTriangles ){
        std::cout << "\t(" << v[0] << ", " << v[1] << ", " << v[2] << ")" << std::endl;
    }
    std::cout << "UV triangles: " << std::endl;
    for( auto v : meshInfo.uvTriangles ){
        std::cout << "\t(" << v[0] << ", " << v[1] << ", " << v[2] << ")" << std::endl;
    }
    std::cout << "-------------------------------------------------------" << std::endl;

    generateMeshVertexData( meshInfo );
}

void OBJPrimitivesImporter::processMeshFileLine( std::string line, MeshInfo& meshInfo )
{
    unsigned int i;
    unsigned int componentsRead = 0;

    if( !line.size() ){
        return;
    }

    std::cout << "line.substr( 0, 2 ): [" << line.substr( 0, 2 ) << "]" << std::endl;

    if( line.substr( 0, 2 ) == "v " ){
        glm::vec3 vertex;

        // Set '.' as the float separator (for parsing floats from a text
        // line).
        setlocale( LC_NUMERIC, "C" );

        // Extract the vertex from the line and add it to the Mesh.
        sscanf( line.c_str(), "v %f %f %f", &vertex[0], &vertex[1], &vertex[2] );

        // Resize the vertex coordinates.
        //vertex *= 0.5f;

        meshInfo.vertices.push_back( vertex );
    }else if( line.substr( 0, 2 ) == "vt" ){
        glm::vec2 textureCoordinates;

        // Extract the UV coordinates from the line and add it to the Mesh.
        sscanf( line.c_str(), "vt %f %f", &textureCoordinates[0], &textureCoordinates[1] );

        meshInfo.uvCoordinates.push_back( textureCoordinates );
    }else if( line.substr( 0, 2 ) == "f " ){
        // TODO: Process multiple types of "face lines".
        std::array< GLuint, 3 > vertexTriangle;

        std::string firstTriangleVertexStr = line.substr( 2, line.find( ' ', 2 ) );
        size_t firstSlashPosition = std::string::npos;
        size_t secondSlashPosition = std::string::npos;

        firstSlashPosition = firstTriangleVertexStr.find( '/' );
        if( firstSlashPosition == std::string::npos ){
            // No slashes found. Face defines only vertex positions.

            componentsRead = sscanf( line.c_str(), "f %u %u %u", &vertexTriangle[0], &vertexTriangle[1], &vertexTriangle[2] );

            if( componentsRead != 3 ){
                throw std::runtime_error( "Error reading mesh triangle (Is it a textured mesh?)" );
            }

            for( i=0; i<3; i++ ){
                // Decrement every vertex index because they are 1-based in the .obj file.
                vertexTriangle[i] -= 1;
            }

            meshInfo.vertexTriangles.push_back( vertexTriangle );

        }else{
            std::array< GLuint, 3 > uvTriangle;

            secondSlashPosition = firstTriangleVertexStr.find( '/', firstSlashPosition + 1 );

            if( secondSlashPosition == std::string::npos ){
                // Only one slash found per face vertex. Face defines vertex
                // position and UV coordinates.

                // Extract the UV coordinates from the line and add it to the Mesh.
                std::cout << "sscanf: " << sscanf( line.c_str(), "f %u/%u %u/%u %u/%u",
                        &vertexTriangle[0], &uvTriangle[0],
                        &vertexTriangle[1], &uvTriangle[1],
                        &vertexTriangle[2], &uvTriangle[2]) << std::endl;

                for( i=0; i<3; i++ ){
                    // Decrement every vertex index because they are 1-based in the .obj file.
                    vertexTriangle[i] -= 1;
                    uvTriangle[i] -= 1;
                }

                meshInfo.vertexTriangles.push_back( vertexTriangle );
                meshInfo.uvTriangles.push_back( uvTriangle );

            }else{
                throw std::runtime_error(
                            std::string( "OBJPrimitivesImporter doesn't recognize the face line [" ) +
                            line +
                            "]" );
            }
        }
    }
    // TODO: And the normals?
}

typedef std::array< GLuint, 3 > CompoundVertex;

void OBJPrimitivesImporter::generateMeshVertexData( MeshInfo &meshInfo )
{
    std::map< CompoundVertex, GLuint > finalVertices;
    std::map< CompoundVertex, GLuint >::const_iterator finalVerticesIt;
    std::list< std::array< GLuint, 3 > > finalTriangles;
    CompoundVertex compoundVertex;
    std::array< GLuint, 3 > triangle;
    unsigned int currentTriangleIndex = 0;
    unsigned int triangleVertexIndex = 0;

    std::cout << "meshInfo.uvTriangles: " << meshInfo.uvTriangles.size() << std::endl;

    for( currentTriangleIndex = 0; currentTriangleIndex < meshInfo.vertexTriangles.size(); currentTriangleIndex++ ){
        for( triangleVertexIndex = 0; triangleVertexIndex < 3; triangleVertexIndex++ ){
            compoundVertex[0] = meshInfo.vertexTriangles[currentTriangleIndex][triangleVertexIndex];
            compoundVertex[1] = ( meshInfo.normalTriangles.size() ) ? meshInfo.normalTriangles[currentTriangleIndex][triangleVertexIndex] : 0;
            compoundVertex[2] = ( meshInfo.uvTriangles.size() ) ? meshInfo.uvTriangles[currentTriangleIndex][triangleVertexIndex] : 0;

            finalVerticesIt = finalVertices.find( compoundVertex );
            if( finalVerticesIt != finalVertices.end() ){
                triangle[triangleVertexIndex] = finalVerticesIt->second;
            }else{
                finalVertices[compoundVertex] = finalVertices.size() - 1;
                triangle[triangleVertexIndex] = finalVertices[compoundVertex];
            }
        }

        finalTriangles.push_back( triangle );
    }

    std::cout << "Final vertices: ";
    for( auto v : finalVertices ){
        std::cout << "\t(" << v.first[0] << ", " << v.first[1] << ", " << v.first[2] << "): " << v.second << std::endl;
    }

    std::cout << "Final triangles: ";
    for( auto t : finalTriangles ){
        std::cout << "\t(" << t[0] << ", " << t[1] << ", " << t[2] << ")" << std::endl;
    }


    for( auto finalVertex : finalVertices ){
        // Insert vertex position.
        meshInfo.vboData.push_back( meshInfo.vertices[ finalVertex.first[0] ][0] );
        meshInfo.vboData.push_back( meshInfo.vertices[ finalVertex.first[0] ][1] );
        meshInfo.vboData.push_back( meshInfo.vertices[ finalVertex.first[0] ][2] );

        // Insert vertex normal (if exists).
        if( meshInfo.normals.size() ){
            meshInfo.vboData.push_back( meshInfo.normals[ finalVertex.first[1] ][0] );
            meshInfo.vboData.push_back( meshInfo.normals[ finalVertex.first[1] ][1] );
            meshInfo.vboData.push_back( meshInfo.normals[ finalVertex.first[1] ][2] );
        }else{
            meshInfo.vboData.push_back( 0 );
            meshInfo.vboData.push_back( 0 );
            meshInfo.vboData.push_back( 0 );
        }

        // Insert UV coordinates (if exist).
        if( meshInfo.uvCoordinates.size() ){
            meshInfo.vboData.push_back( meshInfo.uvCoordinates[ finalVertex.first[2] ][0] );
            meshInfo.vboData.push_back( meshInfo.uvCoordinates[ finalVertex.first[2] ][1] );
        }
    }


    for( auto finalTriangle : finalTriangles ){
        meshInfo.eboData.push_back( finalTriangle[0] );
        meshInfo.eboData.push_back( finalTriangle[1] );
        meshInfo.eboData.push_back( finalTriangle[2] );
    }

    unsigned int i, j;
    const unsigned int componetstPerVertex = ( meshInfo.uvCoordinates.size() ) ? 8 : 6;
    for( i = 0; i < finalVertices.size(); i++ ){
        std::cout << "\t\t(";
        for( j = 0; j < componetstPerVertex; j++ ){
            std::cout << meshInfo.vboData[i * componetstPerVertex + j] << ", ";
        }
        std::cout << ")" << std::endl;
    }

    for( i = 0; i < finalTriangles.size(); i++ ){
        std::cout << "\t\t\t(" << meshInfo.eboData[i*3] << ", " << meshInfo.eboData[i*3+1] << ", " << meshInfo.eboData[i*3+2] << ")" << std::endl;
    }
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
