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

#ifndef MESH_INFO_HPP
#define MESH_INFO_HPP

#include "mesh_vertex_data.hpp"
#include "mesh_normal_data.hpp"
#include "mesh_texture_data.hpp"
#include "mesh_opengl_data.hpp"
#include "material_info.hpp"



namespace como {

struct MeshInfo {
    MeshVertexData vertexData;
    MeshNormalData normalData;
    MeshTextureData textureData;

    MeshOpenGLData oglData;

    std::vector< MaterialInfo > materialsData;
};


} // namespace como


#endif // MESH_INFO_HPP
