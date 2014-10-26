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

#include "system_primitive_data.hpp"
#include <map>

namespace como {

void SystemPrimitiveData::generatePerVertexNormals( unsigned int wallIndex )
{
    std::map< unsigned int, unsigned int > vertexToNormalIndex;
    unsigned int currentTriangleIndex;
    unsigned int i;

    const unsigned int FIRST_NORMAL_INDEX = normalData.normals.size();
    const unsigned int FIRST_VERTEX_TRIANGLE_INDEX = trianglesGroups[wallIndex].firstTriangleIndex;
    const unsigned int N_VERTEX_TRIANGLES = trianglesGroups[wallIndex].nTriangles;
    const unsigned int LAST_VERTEX_TRIANGLE_INDEX =
            FIRST_VERTEX_TRIANGLE_INDEX +
            N_VERTEX_TRIANGLES;

    for( currentTriangleIndex = FIRST_VERTEX_TRIANGLE_INDEX;
         currentTriangleIndex < LAST_VERTEX_TRIANGLE_INDEX;
         currentTriangleIndex++ ){
        for( i = 0; i < 3; i++ ){
            if( !( vertexToNormalIndex.count( vertexData.vertexTriangles[ currentTriangleIndex ][ i ] ) ) ){
                const unsigned int normalIndex = FIRST_NORMAL_INDEX + vertexToNormalIndex.size();
                vertexToNormalIndex[ vertexData.vertexTriangles[ currentTriangleIndex ][ i ] ] = normalIndex;
            }
        }
    }

    const unsigned int N_NORMALS = vertexToNormalIndex.size();
    normalData.normalTriangles.reserve( normalData.normalTriangles.size() + N_VERTEX_TRIANGLES );

    // Initialize as many null normals as vertices are present in vertex
    // triangles.
    normalData.normals.insert( normalData.normals.end(),
                               N_NORMALS,
                               glm::vec3( 0.0f ) );

    IndicesTriangle vertexTriangle;
    IndicesTriangle normalTriangle;
    glm::vec3 triangleNormal;

    // Compute every vertex normal.
    for( currentTriangleIndex = FIRST_VERTEX_TRIANGLE_INDEX;
         currentTriangleIndex < LAST_VERTEX_TRIANGLE_INDEX;
         currentTriangleIndex++ ){
        vertexTriangle = vertexData.vertexTriangles[currentTriangleIndex];
        normalTriangle = { vertexToNormalIndex.at( vertexTriangle[0] ),
                           vertexToNormalIndex.at( vertexTriangle[1] ),
                           vertexToNormalIndex.at( vertexTriangle[2] ) };

        triangleNormal = glm::cross(
                    vertexData.vertices[ vertexTriangle[1] ] - vertexData.vertices[ vertexTriangle[0] ],
                    vertexData.vertices[ vertexTriangle[2] ] - vertexData.vertices[ vertexTriangle[0] ] );

        assert( normalTriangle[0] < normalData.normals.size() );
        assert( normalTriangle[1] < normalData.normals.size() );
        assert( normalTriangle[2] < normalData.normals.size() );

        normalData.normals[ normalTriangle[0] ] += triangleNormal;
        normalData.normals[ normalTriangle[1] ] += triangleNormal;
        normalData.normals[ normalTriangle[2] ] += triangleNormal;

        normalData.normalTriangles.push_back( normalTriangle );
    }
}


void SystemPrimitiveData::generatePerFaceNormals( unsigned int wallIndex )
{
    unsigned int currentTriangleIndex;
    const unsigned int FIRST_VERTEX_TRIANGLE_INDEX = trianglesGroups[wallIndex].firstTriangleIndex;
    const unsigned int N_VERTEX_TRIANGLES = trianglesGroups[wallIndex].nTriangles;
    const unsigned int LAST_VERTEX_TRIANGLE_INDEX =
            FIRST_VERTEX_TRIANGLE_INDEX +
            N_VERTEX_TRIANGLES;

    for( currentTriangleIndex = FIRST_VERTEX_TRIANGLE_INDEX;
         currentTriangleIndex < LAST_VERTEX_TRIANGLE_INDEX;
         currentTriangleIndex++ ){
        const IndicesTriangle vertexTriangle =
                vertexData.vertexTriangles[ currentTriangleIndex ];

        glm::vec3 triangleNormal =
                glm::normalize(
                    glm::cross(
                        vertexData.vertices[ vertexTriangle[1] ] - vertexData.vertices[ vertexTriangle[0] ],
                        vertexData.vertices[ vertexTriangle[2] ] - vertexData.vertices[ vertexTriangle[0] ] ) );

        normalData.normals.push_back( triangleNormal );

        const unsigned int normalIndex = normalData.normals.size() - 1;

        normalData.normalTriangles.push_back({ normalIndex,
                                             normalIndex,
                                             normalIndex });
    }
}

} // namespace como
