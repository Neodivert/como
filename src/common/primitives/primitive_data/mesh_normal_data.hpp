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

#ifndef MESH_NORMAL_DATA_HPP
#define MESH_NORMAL_DATA_HPP

#include <glm/vec3.hpp>
#include <vector>
#include <array>
#include <common/primitives/primitive_data/mesh_vertex_data.hpp>

namespace como {

typedef std::vector< glm::vec3 > NormalsVector;
typedef std::vector< std::array< GLuint, 3 > > NormalTrianglesVector;

struct MeshNormalData {
    NormalsVector normals;
    NormalTrianglesVector normalTriangles;


    /***
     * 1. Construction
     ***/
    MeshNormalData() = default;
    MeshNormalData( const MeshVertexData& meshVertexData );


    /***
     * 2. Initialization
     ***/
    void initFromMeshVertexData( const MeshVertexData& meshVertexData );
};

} // namespace como

#endif // MESH_NORMAL_DATA_HPP
