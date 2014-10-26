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

#include "specialized_system_primitives_factory.hpp"

namespace como {

/***
 * 1. Construction
 ***/

SpecializedSystemPrimitivesFactory::SpecializedSystemPrimitivesFactory( ServerInterfacePtr server, const std::string& primitiveName, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, TextureWallsManager* textureWallsManager ) :
    ServerWriter( server ),
    primitiveName_( primitiveName ),
    meshesManager_( meshesManager ),
    textureWallsManager_( textureWallsManager ),
    materialsManager_( materialsManager )
{}


/***
 * 4. SystemPrimitiveData generation
 ***/

SystemPrimitiveData SpecializedSystemPrimitivesFactory::generatePrimitiveData()
{
    SystemPrimitiveData primitiveData;

    primitiveData.name = primitiveName_;
    generateVerticesPositionsAndUV( primitiveData.vertexData.vertices,
                                    primitiveData.uvData.uvVertices );
    generateWalls( primitiveData );
    generateOGLData( primitiveData );

    return primitiveData;
}


void SpecializedSystemPrimitivesFactory::generateOGLData( SystemPrimitiveData &primitiveData )
{
    primitiveData.generateOGLData();
}


/***
 * 5. Auxiliar methods
 ***/

unsigned int SpecializedSystemPrimitivesFactory::generateHorizontalVerticesCircle(std::vector<glm::vec3> &vertices, float radius, unsigned int nDivisions, float height)
{
    const float angleStep = 2.0f * glm::pi<float>() / nDivisions;
    float currentAngle = 0.0f;
    unsigned int i;
    const unsigned int CENTER_VERTEX_INDEX = vertices.size();

    // Create center vertex
    vertices.push_back( glm::vec3( 0.0f,
                                   height,
                                   0.0f ) );

    // Create radial vertices.
    for( i = 0; i < nDivisions; i++ ){
        vertices.push_back(
                    glm::vec3( radius * cos( currentAngle ),
                               height,
                               radius * sin( currentAngle ) ) );
        currentAngle += angleStep;
    }

    return CENTER_VERTEX_INDEX;
}


unsigned int SpecializedSystemPrimitivesFactory::generateHorizontalUVCircle(std::vector<glm::vec2> &vertices, unsigned int nDivisions)
{
    const float angleStep = 2.0f * glm::pi<float>() / (float)( nDivisions );
    float currentAngle = 0.0f;
    unsigned int i;
    const unsigned int CENTER_VERTEX_INDEX = vertices.size();

    // Create center vertex
    vertices.push_back( glm::vec2( 0.5f, 0.5f ) );

    // Create radial vertices.
    for( i = 0; i < nDivisions; i++ ){
        vertices.push_back(
                    glm::vec2( 0.5f + 0.5f * cos( currentAngle ),
                               0.5f + 0.5f * sin( currentAngle ) ) );
        currentAngle += angleStep;
    }

    return CENTER_VERTEX_INDEX;
}


void SpecializedSystemPrimitivesFactory::generateTrianglesCircle( SystemPrimitiveData& primitiveData, unsigned int nDivisions, unsigned int centerVertexIndex, unsigned int firstRadialVertexIndex, bool increaseIndices )
{
    unsigned int i;

    std::vector< IndicesTriangle > vertexTriangles;

    // Generate the position and UV triangles of the requested circle.
    generateTrianglesCircle( vertexTriangles,
                             nDivisions,
                             centerVertexIndex,
                             firstRadialVertexIndex,
                             increaseIndices );

    // Insert all vertex, UV triangles into the struct.
    for( i = 0; i < vertexTriangles.size(); i++ ){
        primitiveData.addTriangle( vertexTriangles[i],
                                   vertexTriangles[i] );
    }
}


void SpecializedSystemPrimitivesFactory::generateTrianglesCircle( std::vector< IndicesTriangle >& triangles, unsigned int nDivisions, unsigned int centerVertexIndex, unsigned int firstRadialVertexIndex, bool increaseIndices )
{
    unsigned int i;
    unsigned int currentVertexIndex;

    if( increaseIndices ){
        for( i = 0; i < nDivisions - 1; i++ ){
            currentVertexIndex = firstRadialVertexIndex + i;
            triangles.push_back({ centerVertexIndex,
                                  currentVertexIndex,
                                  currentVertexIndex + 1 });
        }
        currentVertexIndex = firstRadialVertexIndex + i;
        triangles.push_back({ centerVertexIndex,
                              currentVertexIndex,
                              firstRadialVertexIndex });
    }else{
        for( i = 0; i < nDivisions - 1; i++ ){
            currentVertexIndex = firstRadialVertexIndex + i;
            triangles.push_back({ centerVertexIndex,
                                  currentVertexIndex + 1,
                                  currentVertexIndex });
        }
        currentVertexIndex = firstRadialVertexIndex + i;
        triangles.push_back({ centerVertexIndex,
                              firstRadialVertexIndex,
                              currentVertexIndex });
    }
}

} // namespace como
