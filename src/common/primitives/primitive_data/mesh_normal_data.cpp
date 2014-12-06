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

#include "mesh_normal_data.hpp"
#define GLM_FORCE_RADIANS
#include <glm/gtx/vector_angle.hpp>

namespace como {

/***
 * 1. Construction
 ***/

MeshNormalData::MeshNormalData( const MeshVertexData& meshVertexData )
{
    initFromMeshVertexData( meshVertexData );
}


/***
 * 2. Initialization
 ***/

void MeshNormalData::initFromMeshVertexData( const MeshVertexData& meshVertexData )
{
    normals.clear();
    normalTriangles.clear();

    glm::vec3 faceNormal;

    // Set a zero normal for every vertex in the mesh.
    normals.resize( meshVertexData.vertices.size() );
    for( glm::vec3& normal : normals ){
        normal = glm::vec3( 0.0f );
    }

    // Compute each face normal.
    for( const IndicesTriangle& triangle : meshVertexData.vertexTriangles ){
        faceNormal = glm::cross(
                    meshVertexData.vertices[ triangle[1] ] - meshVertexData.vertices[ triangle[0] ],
                    meshVertexData.vertices[ triangle[2] ] - meshVertexData.vertices[ triangle[0] ] );

        if( faceNormal.length() > 0.0f ){
            faceNormal = glm::normalize( faceNormal );
        }

        normals[ triangle[0] ] = faceNormal;
        normals[ triangle[1] ] = faceNormal;
        normals[ triangle[2] ] = faceNormal;
    }

    normalTriangles = meshVertexData.vertexTriangles;
}

} // namespace como
