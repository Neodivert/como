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

#ifndef TEXTURE_WALLS_MANAGER_HPP
#define TEXTURE_WALLS_MANAGER_HPP

#include <client/managers/utilities/server_writer.hpp>
#include <common/ids/resource_id.hpp>
#include <client/models/3d/textures/texture_wall.hpp>
#include <map>

namespace como {

class TextureWallsManager;
typedef std::unique_ptr< TextureWallsManager > TextureWallsManagerPtr;

class TextureWallsManager : public ServerWriter
{
    public:
        /***
         * 1. Construction
         ***/
        TextureWallsManager( ServerInterfacePtr server );
        TextureWallsManager() = delete;
        TextureWallsManager( const TextureWallsManager& ) = delete;
        TextureWallsManager( TextureWallsManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~TextureWallsManager() = default;


        /***
         * 3. Texture walls management
         ***/
        ResourceID addTextureWall( TextureWall&& textureWall, const ResourceID& meshID );


        /***
         * 4. Operators
         ***/
        TextureWallsManager& operator = ( const TextureWallsManager& ) = delete;
        TextureWallsManager& operator = ( TextureWallsManager&& ) = delete;


    private:
        std::map< ResourceID, TextureWall > textureWalls_;
        std::map< ResourceID, std::vector< ResourceID > > meshesTextureWalls_;
};

} // namespace como

#endif // TEXTURE_WALLS_MANAGER_HPP
