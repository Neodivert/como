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
#include <common/primitives/primitive_data/system_primitive_data.hpp>
#include "triangles_group_with_texture_wall.hpp"
#include <client/managers/managers/textures/texture_walls_manager.hpp>

namespace como {

// TODO: Rename?
class SystemMesh : public Mesh
{
    public:
        /***
         * 1. Construction
         ***/
        SystemMesh( const ResourceID& meshID, const ResourceID& firstMaterialID, const ResourceID& firstTextureWallID, const SystemPrimitiveData& primitiveData, MaterialsManager& materialsManager, bool displayVertexNormals = false );
        SystemMesh() = delete;
        SystemMesh( const SystemMesh& ) = delete;
        SystemMesh( SystemMesh&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~SystemMesh() = default;


        /***
         * 3. Setters
         ***/
        static void setTextureWallsManager( TextureWallsManager& textureWallsManager );


        /***
         * 4. Drawing
         ***/
        virtual void draw( OpenGLPtr openGL, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix, const glm::vec4 *contourColor ) const;


        /***
         * 5. Operators
         ***/
        SystemMesh& operator = ( const SystemMesh& ) = delete;
        SystemMesh& operator = ( SystemMesh&& ) = delete;

    private:
        std::vector< Texture > textures_;
        std::vector< TrianglesGroupWithTextureWall > trianglesGroups_;

        static TextureWallsManager* textureWallsManager_;
};

} // namespace como

#endif // SYSTEM_MESH_HPP
