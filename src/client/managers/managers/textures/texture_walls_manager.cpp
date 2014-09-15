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

#include "texture_walls_manager.hpp"

namespace como {


/***
 * 1. Construction
 ***/

TextureWallsManager::TextureWallsManager( ServerInterfacePtr server ) :
    ServerWriter( server )
{}


/***
 * 3. Texture walls management
 ***/

ResourceID TextureWallsManager::addTextureWall( TextureWall &&textureWall, const ResourceID &meshID )
{
    ResourceID textureWallID = newResourceID();

    textureWalls_[textureWallID] = std::move( textureWall );

    if( meshesTextureWalls_.count( meshID ) == 0 ){
        meshesTextureWalls_[meshID] = std::vector< ResourceID >();
    }
    meshesTextureWalls_.at( meshID ).push_back( textureWallID );

    return textureWallID;
}

} // namespace como
