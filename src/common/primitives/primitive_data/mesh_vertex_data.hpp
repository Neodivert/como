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

#ifndef MESH_VERTEX_DATA_HPP
#define MESH_VERTEX_DATA_HPP

#include <glm/vec3.hpp>
#include <vector>
#include <array>

extern "C" {
    #include <GL/gl.h>
}

namespace como {

typedef glm::vec3 Vertex;
typedef std::array< GLuint, 3 > IndicesTriangle;
typedef std::array< GLuint, 4 > IndicesQuad;

typedef std::vector< Vertex > VerticesVector;
typedef std::vector< IndicesTriangle > IndicesTrianglesVector;

struct MeshVertexData {
    VerticesVector vertices;
    IndicesTrianglesVector vertexTriangles;

    inline glm::vec3 centroid() const;
};


glm::vec3 MeshVertexData::centroid() const
{
    glm::vec3 centroid( 0.0f );

    for( const glm::vec3& v : vertices ){
        centroid += v;
    }
    if( vertices.size() > 0 ){
        centroid /= vertices.size();
    }

    return centroid;
}

} // namespace como

#endif // MESH_VERTEX_DATA_HPP
