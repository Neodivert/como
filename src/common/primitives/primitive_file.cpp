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

#include "primitive_file.hpp"
#include <common/exceptions/file_not_open_exception.hpp>
#include <fstream>

namespace como {

/***
 * 3. File writting
 ***/

void PrimitiveFile::write( const MeshInfo meshInfo, std::string filePath )
{
    std::ofstream file;

    file.open( filePath );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    writeVertices( meshInfo.vertexData.vertices, file );
    writeTriangles( meshInfo.vertexData.vertexTriangles, file );
    writeOpenGLData( meshInfo.oglData, file );
    writeMaterials( meshInfo.materialsData, file );

    file.close();
}


void PrimitiveFile::writeVertices( const VerticesVector& vertices, std::ofstream& file )
{
    // Write the number of vertices in the mesh.
    file << vertices.size() << std::endl;

    // Write all the vertices to the file (one vertex per line).
    for( auto vertex : vertices ){
        file << vertex[0] << " " << vertex[1] << " " << vertex[2] << std::endl;
    }
}


void PrimitiveFile::writeTriangles( const TrianglesVector& triangles, std::ofstream& file )
{
    // Write the number of triangles in the mesh.
    file << triangles.size() << std::endl;

    // Write all the triangles to the file (one triangle per line).
    for( auto triangle : triangles ){
        file << triangle[0] << " " << triangle[1] << " " << triangle[2] << std::endl;
    }
}


void PrimitiveFile::writeOpenGLData( const MeshOpenGLData& oglData, std::ofstream& file )
{
    unsigned int vertexIndex = 0;
    unsigned int componentIndex = 0;
    unsigned int nVBOVertices = 0;
    unsigned int componentsPerVertex = oglData.getComponentsPerVertex();
    unsigned int triangleIndex = 0;

    nVBOVertices = oglData.vboData.size() / componentsPerVertex;

    // Write the number of components per vertex.
    file << componentsPerVertex << std::endl;

    // Write the number of VBO vertices.
    file << nVBOVertices << std::endl;

    // Write the VBO data to the file (one vertex per line).
    for( vertexIndex = 0; vertexIndex < nVBOVertices; vertexIndex++ ){
        for( componentIndex = 0; componentIndex < oglData.getComponentsPerVertex()-1; componentIndex++ ){
            file << oglData.vboData[vertexIndex * componentsPerVertex + componentIndex] << " ";
        }
        file << oglData.vboData[vertexIndex * componentsPerVertex + componentIndex] << std::endl;
    }

    // Write the number of triangles to the file.
    file << ( oglData.eboData.size() / 3 ) << std::endl;

    // Write the EBO data to the file (one triangle per line).
    for( triangleIndex = 0; triangleIndex < oglData.eboData.size() / 3; triangleIndex++ ){
        file << oglData.eboData[triangleIndex * 3] << " "
             << oglData.eboData[triangleIndex * 3 + 1] << " "
             << oglData.eboData[triangleIndex * 3 + 2] << std::endl;
    }
}


void PrimitiveFile::writeMaterials( const std::vector<MaterialInfo>& materials, std::ofstream& file )
{
    file << materials.size() << std::endl;

    for( auto material : materials ){
        writeMaterial( material, file );
    }
}




void PrimitiveFile::writeMaterial( const MaterialInfo &material, std::ofstream &file )
{
    file << material.name << std::endl;

    file << material.ambientReflectivity[0] << " "
         << material.ambientReflectivity[1] << " "
         << material.ambientReflectivity[2] << std::endl;

    file << material.diffuseReflectivity[0] << " "
         << material.diffuseReflectivity[1] << " "
         << material.diffuseReflectivity[2] << std::endl;

    file << material.specularReflectivity[0] << " "
         << material.specularReflectivity[1] << " "
         << material.specularReflectivity[2] << std::endl;

    file << material.specularExponent << std::endl;

    if( material.textureInfo ){
        file << "1" << std::endl
             << material.textureInfo->imageFileData << std::endl;
    }else{
        file << "0" << std::endl;
    }
}


/***
 * 4. File reading
 ***/

void PrimitiveFile::read( MeshInfo &meshInfo, std::string filePath )
{
    // TODO?: Here we don't use meshInfo.normalData or meshInfo.textureData.
    // Do we use an auxiliar struct without those attributes?

    std::ifstream file;

    file.open( filePath );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    readVertices( meshInfo.vertexData.vertices, file );
    readTriangles( meshInfo.vertexData.vertexTriangles, file );
    readOpenGLData( meshInfo.oglData, file );
    readMaterials( meshInfo.materialsData, file );


    file.close();
}


void PrimitiveFile::readVertices( VerticesVector& vertices, std::ifstream &file )
{
    unsigned int i, nVertices = 0;
    std::string fileLine;
    glm::vec3 vertex;

    // Read the number of vertices in the mesh.
    std::getline( file, fileLine );
    nVertices = atoi( fileLine.c_str() );

    // Read all the vertices from the file (one vertex per line).
    vertices.reserve( nVertices );
    for( i = 0; i < nVertices; i++ ){
        std::getline( file, fileLine );
        sscanf( fileLine.c_str(), "%f %f %f", &vertex[0], &vertex[1], &vertex[2] );
        vertices.push_back( vertex );
    }
}


void PrimitiveFile::readTriangles( VertexTrianglesVector& triangles, std::ifstream& file )
{
    unsigned int i, nTriangles = 0;
    std::string fileLine;
    std::array< GLuint, 3 > triangle;

    // Read the number of triangles in the mesh.
    std::getline( file, fileLine );
    nTriangles = atoi( fileLine.c_str() );

    // Read all the triangles from the file (one triangle per line).
    triangles.reserve( nTriangles );
    for( i = 0; i < nTriangles; i++ ){
        std::getline( file, fileLine );
        sscanf( fileLine.c_str(), "%u %u %u", &triangle[0], &triangle[1], &triangle[2] );
        triangles.push_back( triangle );
    }
}


void PrimitiveFile::readOpenGLData( MeshOpenGLData& oglData, std::ifstream& file )
{
    std::string fileLine;
    unsigned int vertexIndex = 0;
    unsigned int componentIndex = 0;
    unsigned int nVBOVertices = 0;
    unsigned int componentsPerVertex = 0;
    unsigned int triangleIndex = 0;
    unsigned int nTriangles = 0;
    std::array< GLuint, 3 > triangle;
    std::string lineTail;
    std::string token;

    // Set '.' as the float separator (for parsing floats from a text
    // line).
    setlocale( LC_NUMERIC, "C" );

    // Read the number of components per vertex.
    std::getline( file, fileLine );
    componentsPerVertex = atoi( fileLine.c_str() );

    // Read the number of VBO vertices.
    std::getline( file, fileLine );
    nVBOVertices = atoi( fileLine.c_str() );

    // Read the VBO data from the file (one vertex per line).
    for( vertexIndex = 0; vertexIndex < nVBOVertices; vertexIndex++ ){
        std::getline( file, fileLine );
        lineTail = fileLine;

        for( componentIndex = 0; componentIndex < componentsPerVertex - 1; componentIndex++ ){
            token = lineTail.substr( 0, lineTail.find( ' ' ) );

            oglData.vboData.push_back( atof( token.c_str() ) );

            lineTail = lineTail.substr( lineTail.find( ' ' ) + 1 );
        }
        oglData.vboData.push_back( atof( lineTail.c_str() ) );
    }

    // Read the number of triangles from the file.
    std::getline( file, fileLine );
    nTriangles = atoi( fileLine.c_str() );

    // Read the EBO data from the file (one triangle per line).
    oglData.eboData.reserve( nTriangles );
    for( triangleIndex = 0; triangleIndex < nTriangles; triangleIndex++ ){
        std::getline( file, fileLine );
        sscanf( fileLine.c_str(), "%u %u %u",
                &triangle[0],
                &triangle[1],
                &triangle[2] );

        oglData.eboData.push_back( triangle[0] );
        oglData.eboData.push_back( triangle[1] );
        oglData.eboData.push_back( triangle[2] );
    }
}


void PrimitiveFile::readMaterials( std::vector<MaterialInfo>& materials, std::ifstream& file )
{
    std::string fileLine;
    unsigned int i = 0, nMaterials = 0;

    // Read the number of materials.
    std::getline( file, fileLine );
    nMaterials = atoi( fileLine.c_str() );

    // Read all the materials.
    materials.resize( nMaterials );
    for( i = 0; i < nMaterials; i++ ){
        readMaterial( materials[i], file );
    }
}


void PrimitiveFile::readMaterial( MaterialInfo &material, std::ifstream &file )
{
    std::string fileLine;

    // Read the material's name.
    std::getline( file, material.name );

    // Read the material's ambient reflectivity.
    std::getline( file, fileLine );
    material.ambientReflectivity = readVec3( fileLine );

    // Read the material's diffuse reflectivity.
    std::getline( file, fileLine );
    material.diffuseReflectivity = readVec3( fileLine );

    // Read the material's specular reflectivity.
    std::getline( file, fileLine );
    material.specularReflectivity = readVec3( fileLine );

    // Read the material's specular exponent.
    std::getline( file, fileLine );
    material.specularExponent = atoi( fileLine.c_str() );

    // Next line is "1" if the current material contains a texture or "0"
    // otherwise.
    std::getline( file, fileLine );
    if( fileLine == "1" ){
        // Read material info.
        material.textureInfo->imageFileData = fileLine;
    }
}


/***
 * 5. Auxiliar methods
 ***/

glm::vec3 PrimitiveFile::readVec3( std::string str )
{
    glm::vec3 v;

    sscanf( str.c_str(), "%f %f %f", &v[0], &v[1], &v[2] );

    return v;
}

} // namespace como
