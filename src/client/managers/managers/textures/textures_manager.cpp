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

#include "textures_manager.hpp"

namespace como {


/***
 * 1. Construction
 ***/

TexturesManager::TexturesManager( ServerInterfacePtr server, const std::string& sceneDirPath ) :
    AbstractTexturesManager( sceneDirPath ),
    ServerWriter( server )
{}


/***
 * 3. Textures management
 ***/

ResourceID TexturesManager::loadTexture( std::string imagePath )
{
    (void)( imagePath );
    // TODO: Complete

    return NO_RESOURCE;
}


/***
 * 4. Local texture walls management
 ***/

ResourceID TexturesManager::createTextureWall( string name )
{
    ResourceID textureWallID = reserveResourceIDs( 1 );

    textureWalls_.emplace( textureWallID, name );

    return textureWallID;
}


/***
 * 4. Shader communication
 ***/

void TexturesManager::sendTextureToShader( const ResourceID& resourceID ) const
{
    textures_.at( resourceID ).sendToShader();
}

} // namespace como
