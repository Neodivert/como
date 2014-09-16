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
 * 3. Getters
 ***/

bool TexturesManager::textureWallIncludesTexture( const ResourceID& textureWallID ) const
{
    return ( textureWalls_.at( textureWallID ).textureID != NO_RESOURCE );
}


/***
 * 4. Local textures management
 ***/

ResourceID TexturesManager::loadTexture( std::string imagePath )
{
    (void)( imagePath );
    // TODO: Complete

    return NO_RESOURCE;
}


/***
 * 5. Local texture walls management
 ***/

ResourceID TexturesManager::createTextureWall( string name )
{
    ResourceID textureWallID = reserveResourceIDs( 1 );

    textureWalls_.emplace( textureWallID, name );

    return textureWallID;
}


/***
 * 6. Shader communication
 ***/

void TexturesManager::sendTextureWallToShader( const ResourceID &resourceID ) const
{
    //textureWalls_.at( resourceID ).sendToShader( *this );
    // TODO: Send texture offset and scale.

    sendTextureToShader( textureWalls_.at( resourceID ).textureID );
}


void TexturesManager::sendTextureToShader( const ResourceID& resourceID ) const
{
    textures_.at( resourceID ).sendToShader();
}

} // namespace como
