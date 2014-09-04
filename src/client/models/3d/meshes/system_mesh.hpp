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

#ifndef SYSTEM_MESH_HPP
#define SYSTEM_MESH_HPP

#include <client/models/3d/mesh.hpp>

namespace como {

class SystemMesh : public Mesh
{
    public:
        /***
         * 1. Construction
         ***/
        SystemMesh( MeshVertexData vertexData, const MeshOpenGLData& oglData, const std::vector< PolygonGroupData >& polygonsGroups, const std::vector< MaterialConstPtr >& materials, bool displayVertexNormals = false );
        SystemMesh() = delete;
        SystemMesh( const SystemMesh& ) = delete;
        SystemMesh( SystemMesh&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~SystemMesh() = default;


        /***
         * 3. Wall
         ***/



        /***
         * 4. Operators
         ***/
        SystemMesh& operator = ( const SystemMesh& ) = delete;
        SystemMesh& operator = ( SystemMesh&& ) = delete;
};

} // namespace como

#endif // SYSTEM_MESH_HPP
