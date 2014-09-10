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

#include <common/primitives/primitive_data/primitive_data.hpp>

namespace como {


/***
 * 1. Getters
 ***/

unsigned int PrimitiveData::getMaterialIndex( const std::string& name ) const
{
    unsigned int materialIndex = 0;

    for( const auto& material : materialsInfo_ ){
        if( material.name == name ){
            return materialIndex;
        }
        materialIndex++;
    }

    throw std::runtime_error( std::string( "OBJPrimitivesImporter - material [" ) +
                              name +
                              "] not found" );
}


/***
 * 2. File reading
 ***/

void PrimitiveData::readVertices( std::ifstream &file )
{
    unsigned int i, nVertices = 0;
    std::string fileLine;
    glm::vec3 vertex;

    // Read the number of vertices in the mesh.
    std::getline( file, fileLine ); // TODO: Use OBJPrimitivesImporter::readLine() instead?
    nVertices = atoi( fileLine.c_str() );

    // Read all the vertices from the file (one vertex per line).
    vertexData.vertices.reserve( nVertices );
    for( i = 0; i < nVertices; i++ ){
        std::getline( file, fileLine );
        sscanf( fileLine.c_str(), "%f %f %f", &vertex[0], &vertex[1], &vertex[2] );
        vertexData.vertices.push_back( vertex );
    }
}


void PrimitiveData::readTriangles( std::ifstream& file )
{
    unsigned int i, nTriangles = 0;
    std::string fileLine;
    std::array< GLuint, 3 > triangle;

    // Read the number of triangles in the mesh.
    std::getline( file, fileLine );
    nTriangles = atoi( fileLine.c_str() );

    // Read all the triangles from the file (one triangle per line).
    vertexData.vertexTriangles.reserve( nTriangles );
    for( i = 0; i < nTriangles; i++ ){
        std::getline( file, fileLine );
        sscanf( fileLine.c_str(), "%u %u %u", &triangle[0], &triangle[1], &triangle[2] );
        vertexData.vertexTriangles.push_back( triangle );
    }
}


void PrimitiveData::readOpenGLData( std::ifstream& file )
{
    std::string fileLine;
    unsigned int vertexIndex = 0;
    unsigned int componentIndex = 0;
    unsigned int nVBOVertices = 0;
    unsigned int triangleIndex = 0;
    unsigned int nTriangles = 0;
    std::array< GLuint, 3 > triangle;
    std::string lineTail;
    std::string token;

    // Set '.' as the float separator (for parsing floats from a text
    // line).
    setlocale( LC_NUMERIC, "C" );

    // Read the number of components per vertex.
    // Read a boolean indicating if the OGL Data includes texture data.
    std::getline( file, fileLine );
    oglData.includesTextures = atoi( fileLine.c_str() );

    // Read the number of VBO vertices.
    std::getline( file, fileLine );
    nVBOVertices = atoi( fileLine.c_str() );

    // Read the VBO data from the file (one vertex per line).
    for( vertexIndex = 0; vertexIndex < nVBOVertices; vertexIndex++ ){
        std::getline( file, fileLine );
        lineTail = fileLine;

        for( componentIndex = 0; componentIndex < oglData.componentsPerVertex() - 1; componentIndex++ ){
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
    oglData.eboData.reserve( nTriangles * 3 );
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


void PrimitiveData::readMaterials(std::ifstream &file)
{
    std::string fileLine;

    std::getline( file, fileLine );
    materialsInfo_.resize( atoi( fileLine.c_str() ) );

    for( auto& material : materialsInfo_ ){
        material.readFromFile( file );
    }
}


/***
 * 3. File writting
 ***/

void PrimitiveData::writeVertices( std::ofstream& file ) const
{
    // Write the number of vertices in the mesh.
    file << vertexData.vertices.size() << std::endl;

    // Write all the vertices to the file (one vertex per line).
    for( const auto& vertex : vertexData.vertices ){
        file << vertex[0] << " " << vertex[1] << " " << vertex[2] << std::endl;
    }
}


void PrimitiveData::writeTriangles( std::ofstream& file ) const
{
    // Write the number of triangles in the mesh.
    file << vertexData.vertexTriangles.size() << std::endl;

    // Write all the triangles to the file (one triangle per line).
    for( const auto& triangle : vertexData.vertexTriangles ){
        file << triangle[0] << " " << triangle[1] << " " << triangle[2] << std::endl;
    }
}


void PrimitiveData::writeOpenGLData( std::ofstream& file ) const
{
    unsigned int vertexIndex = 0;
    unsigned int componentIndex = 0;
    unsigned int nVBOVertices = 0;
    unsigned int componentsPerVertex = oglData.componentsPerVertex();
    unsigned int triangleIndex = 0;

    nVBOVertices = oglData.vboData.size() / componentsPerVertex;

    // Write a boolean indicating if the OGL Data includes texture data.
    file << oglData.includesTextures << std::endl;

    // Write the number of VBO vertices.
    file << nVBOVertices << std::endl;

    // Write the VBO data to the file (one vertex per line).
    for( vertexIndex = 0; vertexIndex < nVBOVertices; vertexIndex++ ){
        for( componentIndex = 0; componentIndex < oglData.componentsPerVertex()-1; componentIndex++ ){
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


void PrimitiveData::writeMaterials(std::ofstream &file) const
{
    file << materialsInfo_.size() << std::endl;

    for( const auto& material : materialsInfo_ ){
        material.writeToFile( file );
    }
}


} // namespace como
