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
#include <common/primitives/primitive_file.hpp>
#include <map>
#include <array>

namespace como {

/***
 * 3. Primitives import
 ***/

PrimitiveInfo OBJPrimitivesImporter::importPrimitive( std::string srcFilePath, std::string dstDirectory, std::string nameSuffix )
{
    MeshInfo meshInfo;
    PrimitiveInfo primitiveInfo;

    processMeshFile( srcFilePath, primitiveInfo, meshInfo );

    primitiveInfo.name += nameSuffix;
    primitiveInfo.filePath =
            dstDirectory + '/' +
            primitiveInfo.name +
            ".prim";

    // All primitive must have at least one material.
    if( !meshInfo.materialsData.size() ){
        meshInfo.materialsData.push_back( MaterialInfo() );
    }

    PrimitiveFile::write( meshInfo, primitiveInfo.filePath );

    return primitiveInfo;
}


void OBJPrimitivesImporter::processMeshFile( std::string filePath, PrimitiveInfo& primitiveInfo, MeshInfo& meshInfo )
{
    std::ifstream file;
    std::string fileLine;

    file.open( filePath );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    while( !file.eof() ){
        std::getline( file, fileLine );

        processMeshFileLine( filePath, fileLine, primitiveInfo, meshInfo );
    }

    file.close();

    //if( meshInfo.normalData.normals.size() != meshInfo.vertexData.vertices.size() ){
        computeVertexNormals( meshInfo.vertexData, meshInfo.normalData );
    //}

    generateMeshVertexData( meshInfo );
}

void OBJPrimitivesImporter::processMeshFileLine( std::string filePath, std::string line, PrimitiveInfo& primitiveInfo, MeshInfo& meshInfo )
{
    unsigned int i;
    unsigned int componentsRead = 0;
    std::string lineHeader;
    std::string lineBody;

    if( !line.size() ){
        return;
    }

    lineHeader = line.substr( 0, line.find( ' ' ) );
    lineBody = line.substr( line.find( ' ' ) + 1 );
    if( lineHeader == "o" ){
        primitiveInfo.name = lineBody;
    }else if( lineHeader == "v" ){
        glm::vec3 vertex;

        // Set '.' as the float separator (for parsing floats from a text
        // line).
        setlocale( LC_NUMERIC, "C" );

        // Extract the vertex from the line and add it to the Mesh.
        sscanf( lineBody.c_str(), "%f %f %f", &vertex[0], &vertex[1], &vertex[2] );

        // Resize the vertex coordinates.
        //vertex *= 0.5f;

        meshInfo.vertexData.vertices.push_back( vertex );
    }else if( lineHeader == "vt" ){
        glm::vec2 textureCoordinates;

        // Extract the UV coordinates from the line and add it to the Mesh.
        sscanf( lineBody.c_str(), "%f %f", &textureCoordinates[0], &textureCoordinates[1] );

        meshInfo.textureData.uvCoordinates.push_back( textureCoordinates );
    }else if( lineHeader == "f" ){
        // TODO: Process multiple types of "face lines".
        std::array< GLuint, 3 > vertexTriangle;

        std::string firstTriangleVertexStr = line.substr( 2, line.find( ' ', 2 ) );
        size_t firstSlashPosition = std::string::npos;
        size_t secondSlashPosition = std::string::npos;

        firstSlashPosition = firstTriangleVertexStr.find( '/' );
        if( firstSlashPosition == std::string::npos ){
            // No slashes found. Face defines only vertex positions.

            componentsRead = sscanf( lineBody.c_str(), "%u %u %u", &vertexTriangle[0], &vertexTriangle[1], &vertexTriangle[2] );

            if( componentsRead != 3 ){
                throw std::runtime_error( "Error reading mesh triangle (Is it a textured mesh?)" );
            }

            for( i=0; i<3; i++ ){
                // Decrement every vertex index because they are 1-based in the .obj file.
                vertexTriangle[i] -= 1;
            }

            meshInfo.vertexData.vertexTriangles.push_back( vertexTriangle );

        }else{
            std::array< GLuint, 3 > uvTriangle;

            secondSlashPosition = firstTriangleVertexStr.find( '/', firstSlashPosition + 1 );

            if( secondSlashPosition == std::string::npos ){
                // Only one slash found per face vertex. Face defines vertex
                // position and UV coordinates.

                // Extract the UV coordinates from the line and add it to the Mesh.
                componentsRead =
                        sscanf( lineBody.c_str(), "%u/%u %u/%u %u/%u",
                        &vertexTriangle[0], &uvTriangle[0],
                        &vertexTriangle[1], &uvTriangle[1],
                        &vertexTriangle[2], &uvTriangle[2]);

                if( componentsRead != 6 ){
                    throw std::runtime_error( "ERROR reading OBJ face line (v/vt)" );
                }

                for( i=0; i<3; i++ ){
                    // Decrement every vertex index because they are 1-based in the .obj file.
                    vertexTriangle[i] -= 1;
                    uvTriangle[i] -= 1;
                }

                meshInfo.vertexData.vertexTriangles.push_back( vertexTriangle );
                meshInfo.textureData.uvTriangles.push_back( uvTriangle );

            }else{
                throw std::runtime_error(
                            std::string( "OBJPrimitivesImporter doesn't recognize the face line [" ) +
                            line +
                            "]" );
            }
        }
    }else if( lineHeader == "mtllib" ){
        std::string fileDirectory = filePath.substr( 0, filePath.rfind( '/' ) );
        std::string materialFilePath = fileDirectory + '/' + lineBody;
        processMaterialFile( materialFilePath, meshInfo.materialsData );
    }
    // TODO: And the normals?
}

typedef std::array< GLuint, 3 > CompoundVertex;

void OBJPrimitivesImporter::generateMeshVertexData( MeshInfo &meshInfo )
{
    std::map< CompoundVertex, GLuint > finalVertices;
    std::map< CompoundVertex, GLuint >::const_iterator finalVerticesIt;
    CompoundVertex compoundVertex;
    unsigned int currentTriangleIndex = 0;
    unsigned int triangleVertexIndex = 0;
    GLuint compoundVertexIndex = 0;

    meshInfo.oglData.includesTextures = ( meshInfo.textureData.uvCoordinates.size() != 0 );

    for( currentTriangleIndex = 0; currentTriangleIndex < meshInfo.vertexData.vertexTriangles.size(); currentTriangleIndex++ ){
        for( triangleVertexIndex = 0; triangleVertexIndex < 3; triangleVertexIndex++ ){
            compoundVertex[0] = meshInfo.vertexData.vertexTriangles[currentTriangleIndex][triangleVertexIndex];
            compoundVertex[1] = meshInfo.normalData.normalTriangles[currentTriangleIndex][triangleVertexIndex]; // TODO: Remove normal triangles?.
            compoundVertex[2] = ( meshInfo.textureData.uvTriangles.size() ) ? meshInfo.textureData.uvTriangles[currentTriangleIndex][triangleVertexIndex] : 0;

            finalVerticesIt = finalVertices.find( compoundVertex );
            if( finalVerticesIt != finalVertices.end() ){
                compoundVertexIndex = finalVerticesIt->second;
            }else{
                compoundVertexIndex = finalVertices.size();

                meshInfo.oglData.vboData.push_back( meshInfo.vertexData.vertices[ compoundVertex[0] ][0] );
                meshInfo.oglData.vboData.push_back( meshInfo.vertexData.vertices[ compoundVertex[0] ][1] );
                meshInfo.oglData.vboData.push_back( meshInfo.vertexData.vertices[ compoundVertex[0] ][2] );

                // Insert vertex normal.
                meshInfo.oglData.vboData.push_back( meshInfo.normalData.normals[ compoundVertex[1] ][0] );
                meshInfo.oglData.vboData.push_back( meshInfo.normalData.normals[ compoundVertex[1] ][1] );
                meshInfo.oglData.vboData.push_back( meshInfo.normalData.normals[ compoundVertex[1] ][2] );

                // Insert UV coordinates (if exist).
                if( meshInfo.textureData.uvCoordinates.size() ){
                    meshInfo.oglData.vboData.push_back( meshInfo.textureData.uvCoordinates[ compoundVertex[2] ][0] );
                    meshInfo.oglData.vboData.push_back( meshInfo.textureData.uvCoordinates[ compoundVertex[2] ][1] );
                }

                finalVertices.insert( std::pair< CompoundVertex, GLuint >( compoundVertex, compoundVertexIndex ) );
            }
            meshInfo.oglData.eboData.push_back( compoundVertexIndex );
        }
    }
}


void OBJPrimitivesImporter::computeVertexNormals( const MeshVertexData &vertexData, MeshNormalData &normalData )
{
    glm::vec3 faceNormal;

    // Set a zero normal for every vertex in the mesh.
    normalData.normals.resize( vertexData.vertices.size(), glm::vec3( 0.0f ) );

    // Compute each face normal and add it to the normal of every vertex in
    // in the triangle.
    for( auto triangle : vertexData.vertexTriangles ){
        faceNormal = glm::cross(
                    vertexData.vertices[ triangle[2] ] - vertexData.vertices[ triangle[0] ],
                    vertexData.vertices[ triangle[1] ] - vertexData.vertices[ triangle[0] ] );

        normalData.normals[ triangle[0] ] += faceNormal;
        normalData.normals[ triangle[1] ] += faceNormal;
        normalData.normals[ triangle[2] ] += faceNormal;
    }

    // Normalize all the normals.
    for( auto& normal : normalData.normals ){
        normal = glm::normalize( normal );
    }

    normalData.normalTriangles = vertexData.vertexTriangles;
}


void OBJPrimitivesImporter::processMaterialFile( std::string filePath, std::vector<MaterialInfo> &materials )
{
    std::ifstream file;
    std::string fileLine;

    file.open( filePath );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    while( !file.eof() ){
        std::getline( file, fileLine );

        processMaterialFileLine( fileLine, materials );
    }

    file.close();
}


void OBJPrimitivesImporter::processMaterialFileLine( std::string fileLine, std::vector<MaterialInfo>& materials )
{
    std::string lineHeader = fileLine.substr( 0, fileLine.find( ' ' ) );
    std::string lineBody = fileLine.substr( fileLine.find( ' ' ) + 1 );

    if( !fileLine.size() ){
        return;
    }

    // Retrieve material parameters from file.
    if( lineHeader == "newmtl" ){
        MaterialInfo newMaterial;
        newMaterial.name = lineBody;
        materials.push_back( newMaterial );
    }else if( lineHeader == "Ka" ){
        materials.back().ambientReflectivity = PrimitiveFile::readVec3( lineBody );
    }else if( lineHeader == "Kd" ){
        materials.back().diffuseReflectivity = PrimitiveFile::readVec3( lineBody );
    }else if( lineHeader == "Ks" ){
        materials.back().specularReflectivity = PrimitiveFile::readVec3( lineBody );
    }else if( lineHeader == "Ns" ){
        materials.back().specularExponent = std::atof( lineBody.c_str() );
    }
    // TODO: Import texture (map_Kd).
}


void OBJPrimitivesImporter::importTextureFile( PrimitiveInfo &primitive, std::string srcFilePath, std::string dstDirectory )
{
    (void)( primitive );
    (void)( srcFilePath );
    (void)( dstDirectory );
    /*
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
    */
}


} // namespace como
