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

#ifndef TRIANGLES_GROUP_WITH_TEXTURE_WALL_HPP
#define TRIANGLES_GROUP_WITH_TEXTURE_WALL_HPP

#include <client/managers/managers/textures/texture_walls_manager.hpp>
#include <common/primitives/primitive_data/triangles_group.hpp>
#include <common/3d/texture_wall.hpp>
#include <client/models/3d/textures/texture.hpp>

#include <common/ids/resource_id.hpp>


namespace como {

struct TrianglesGroupWithTextureWall : public TrianglesGroup
{
    public:
        ResourceID textureWallID;


        /***
         * 1. Construction
         ***/
        TrianglesGroupWithTextureWall( const ResourceID& textureWallID,
                                       unsigned int firstTriangleIndex = 0,
                                       unsigned int nTriangles = 0 );

        TrianglesGroupWithTextureWall( const TrianglesGroupWithTextureWall& ) = default;
        TrianglesGroupWithTextureWall( TrianglesGroupWithTextureWall&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~TrianglesGroupWithTextureWall() = default;


        /***
         * 3. Operators
         ***/
        TrianglesGroupWithTextureWall& operator = ( const TrianglesGroupWithTextureWall& ) = default;
        TrianglesGroupWithTextureWall& operator = ( TrianglesGroupWithTextureWall&& ) = default;
};

} // namespace como

#endif // TRIANGLES_GROUP_WITH_TEXTURE_WALL_HPP
