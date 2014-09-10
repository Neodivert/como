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

#ifndef MESH_TEXTURE_DATA_HPP
#define MESH_TEXTURE_DATA_HPP

#include <glm/glm.hpp>
#include <vector>
#include <array>

namespace como {

typedef std::vector< glm::vec2 > UVCoordinatesVector;
typedef std::vector< std::array< GLuint, 3 > > UVTrianglesVector;

struct MeshTextureData {
    UVCoordinatesVector uvVertices;
    UVTrianglesVector uvTriangles;
};

} // namespace como

#endif // MESH_TEXTURE_DATA_HPP
