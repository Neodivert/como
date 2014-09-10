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

#include "cube.hpp"

typedef std::array< GLuint, 4 > IndicesQuad;

namespace como {

/***
 * 1. Construction
 ***/

Cube::Cube( float width, float height, float depth )
{
    MeshVertexData meshVertexData =
            generateVertexData( width, height, depth );

    MeshNormalData meshNormalData( meshVertexData );

    (void)( meshVertexData );
    (void)( meshNormalData );
}


/***
 * 4. Initialization
 ***/

MeshVertexData Cube::generateVertexData( float width, float height, float depth )
{
    MeshVertexData meshVertexData;

    // Front face (vertices)
    meshVertexData.vertices.push_back( glm::vec3( width / 2.0f, height / 2.0f, depth / 2.0f ) );
    meshVertexData.vertices.push_back( glm::vec3( -width / 2.0f, height / 2.0f, depth / 2.0f ) );
    meshVertexData.vertices.push_back( glm::vec3( -width / 2.0f, -height / 2.0f, depth / 2.0f ) );
    meshVertexData.vertices.push_back( glm::vec3( width / 2.0f, -height / 2.0f, depth / 2.0f ) );

    // Back face (vertices)
    meshVertexData.vertices.push_back( glm::vec3( width / 2.0f, height / 2.0f, -depth / 2.0f ) );
    meshVertexData.vertices.push_back( glm::vec3( -width / 2.0f, height / 2.0f, -depth / 2.0f ) );
    meshVertexData.vertices.push_back( glm::vec3( -width / 2.0f, -height / 2.0f, -depth / 2.0f ) );
    meshVertexData.vertices.push_back( glm::vec3( width / 2.0f, -height / 2.0f, -depth / 2.0f ) );

    // Front face (triangles)
    meshVertexData.vertexTriangles.push_back( IndicesTriangle{ 0, 2, 3 } );
    meshVertexData.vertexTriangles.push_back( IndicesTriangle{ 0, 1, 2 } );

    // Right face (triangles)
    meshVertexData.vertexTriangles.push_back( IndicesTriangle{ 4, 3, 7 } );
    meshVertexData.vertexTriangles.push_back( IndicesTriangle{ 4, 0, 3 } );

    // Left face (triangles)
    meshVertexData.vertexTriangles.push_back( IndicesTriangle{ 1, 6, 2 } );
    meshVertexData.vertexTriangles.push_back( IndicesTriangle{ 1, 5, 6 } );

    // Back face (triangles)
    meshVertexData.vertexTriangles.push_back( IndicesTriangle{ 5, 7, 6 } );
    meshVertexData.vertexTriangles.push_back( IndicesTriangle{ 5, 4, 7 } );

    // Top face (triangles)
    meshVertexData.vertexTriangles.push_back( IndicesTriangle{ 4, 1, 0 } );
    meshVertexData.vertexTriangles.push_back( IndicesTriangle{ 4, 5, 1 } );

    // Bottom face (triangles)
    meshVertexData.vertexTriangles.push_back( IndicesTriangle{ 7, 6, 3 } );
    meshVertexData.vertexTriangles.push_back( IndicesTriangle{ 6, 2, 3 } );

    return meshVertexData;
}


MeshNormalData Cube::generateNormalData( const MeshVertexData& meshVertexData )
{
    return MeshNormalData( meshVertexData );
}


MeshTextureData Cube::generateTextureData()
{
    MeshTextureData meshTextureData;

    // UV - Top right
    meshTextureData.uvVertices.push_back( glm::vec2( 1.0f, 1.0f ) );

    // UV - Top left
    meshTextureData.uvVertices.push_back( glm::vec2( 0.0f, 1.0f ) );

    // UV - Bottom left
    meshTextureData.uvVertices.push_back( glm::vec2( 0.0f, 0.0f ) );

    // UV - Botom right
    meshTextureData.uvVertices.push_back( glm::vec2( 1.0f, 0.0f ) );

    // UV triangle
    meshTextureData.uvTriangles.push_back( IndicesTriangle{ 0, 2, 3 } );
    meshTextureData.uvTriangles.push_back( IndicesTriangle{ 0, 1, 2 } );

    return meshTextureData;
}

} // namespace como
