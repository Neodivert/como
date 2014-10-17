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
#include <array>
#include <map>


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
 * 2. Data generation
 ***/

void PrimitiveData::generateOGLData()
{
    CompoundVerticesMap compoundVerticesMap;
    unsigned int currentTriangleIndex = 0;
    unsigned int currentTriangleElement = 0;

    CompoundVertex compoundVertex;
    CompoundVerticesMap::const_iterator finalVerticesIt;
    VertexIndice compoundVertexIndex;

    oglData.includesTextures = (uvData.uvTriangles.size() > 0);

    if( vertexData.vertexTriangles.size() != normalData.normalTriangles.size() ){
        throw std::runtime_error(
                    "vertexTriangles != normalTriangles (" +
                    std::to_string( vertexData.vertexTriangles.size() ) +
                    " != " +
                    std::to_string( normalData.normalTriangles.size() ) );
    }
    if( oglData.includesTextures && ( vertexData.vertexTriangles.size() != uvData.uvTriangles.size() ) ){
        throw std::runtime_error(
                    "vertexTriangles != uvTriangles (" +
                    std::to_string( vertexData.vertexTriangles.size() ) +
                    " != " +
                    std::to_string( uvData.uvTriangles.size() ) );
    }

    for( currentTriangleIndex = 0; currentTriangleIndex < vertexData.vertexTriangles.size(); currentTriangleIndex++ ){
        for( currentTriangleElement = 0; currentTriangleElement < 3; currentTriangleElement++ ){
            compoundVertex[0] = vertexData.vertexTriangles[currentTriangleIndex][currentTriangleElement];
            compoundVertex[1] = normalData.normalTriangles[currentTriangleIndex][currentTriangleElement];
            compoundVertex[2] = ( oglData.includesTextures ? uvData.uvTriangles[currentTriangleIndex][currentTriangleElement] : 0 );

            finalVerticesIt = compoundVerticesMap.find( compoundVertex );

            if( finalVerticesIt != compoundVerticesMap.end() ){
                compoundVertexIndex = finalVerticesIt->second;
            }else{
                compoundVertexIndex = compoundVerticesMap.size();

                oglData.vboData.push_back( vertexData.vertices[ compoundVertex[0] ][0] );
                oglData.vboData.push_back( vertexData.vertices[ compoundVertex[0] ][1] );
                oglData.vboData.push_back( vertexData.vertices[ compoundVertex[0] ][2] );

                // Insert vertex normal.
                oglData.vboData.push_back( normalData.normals[ compoundVertex[1] ][0] );
                oglData.vboData.push_back( normalData.normals[ compoundVertex[1] ][1] );
                oglData.vboData.push_back( normalData.normals[ compoundVertex[1] ][2] );

                // Insert UV coordinates (if exist).
                if( oglData.includesTextures ){
                    oglData.vboData.push_back( uvData.uvVertices[ compoundVertex[2] ][0] );
                    oglData.vboData.push_back( uvData.uvVertices[ compoundVertex[2] ][1] );

                    if( compoundVertex[2] > uvData.uvVertices.size() ){
                        throw std::runtime_error(
                                    "compoundVertex[2] > uvData.uvVertices.size()" +
                                    std::to_string( compoundVertex[2] ) +
                                    " > " +
                                    std::to_string( uvData.uvVertices.size() ) );
                    }
                }

                compoundVerticesMap.insert( std::pair< CompoundVertex, VertexIndice >( compoundVertex, compoundVertexIndex ) );
            }

            oglData.eboData.push_back( compoundVertexIndex );
        }
    }
}


/***
 * 3. File importing / exporting
 ***/

void PrimitiveData::importFromFile( const std::string &filePath )
{
    std::ifstream file;

    file.open( filePath );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    read( file );

    file.close();
}


void PrimitiveData::exportToFile( const std::string& filePath ) const
{
    std::ofstream file;

    file.open( filePath );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    write( file );

    file.close();
}


/***
 * 4. Auxiliar methods
 ***/

std::string PrimitiveData::getNameFromFile(const std::string &filePath)
{
    std::ifstream file;
    std::string primitiveName;

    file.open( filePath );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    std::getline( file, primitiveName );

    file.close();

    return primitiveName;
}


/***
 * 5. File reading / writing
 ***/

void PrimitiveData::read( std::ifstream &file )
{
    std::getline( file, name );
    readVertices( file );
    readTriangles( file );
    readOpenGLData( file );
    readMaterials( file );
}


void PrimitiveData::write( std::ofstream &file ) const
{
    file << name << std::endl;
    writeVertices( file );
    writeTriangles( file );
    writeOpenGLData( file );
    writeMaterials( file );
}


/***
 * 6. File reading (auxiliar methods)
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
 * 7. File writting (auxiliar methods)
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
