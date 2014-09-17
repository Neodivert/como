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

TextureWallsManager::TextureWallsManager( ServerInterfacePtr server, TexturesManager &texturesManager ) :
    ResourcesOwnershipRequester( server ),
    texturesManager_( &texturesManager )
{}


/***
 * 3. Getters
 ***/

bool TextureWallsManager::textureWallIncludesTexture( const ResourceID& textureWallID ) const
{
    return ( textureWalls_.at( textureWallID ).textureID != NO_RESOURCE );
}


ResourceHeadersList TextureWallsManager::getSelectableTextureWallsHeaders() const
{
    ResourceHeadersList headersList;
    ResourceHeader currentHeader;

    for( const auto& textureWallID : selectableTextureWalls_ ){
        currentHeader.id = textureWallID;
        currentHeader.name = textureWalls_.at( textureWallID ).name;
        headersList.push_back( currentHeader );
    }

    return headersList;
}


/***
 * 4. Local texture walls management
 ***/

ResourceID TextureWallsManager::createTextureWall( string name )
{
    ResourceID textureWallID = reserveResourceIDs( 1 );

    textureWalls_.emplace( textureWallID, name );

    return textureWallID;
}


void TextureWallsManager::toggleTextureWallSeletable( const ResourceID& textureWallID, bool selectable )
{
    if( selectable ){
        selectableTextureWalls_.push_back( textureWallID );
    }else{
        selectableTextureWalls_.remove( textureWallID );
    }
}


/***
 * 5. Resources ownership requesting
 ***/

void TextureWallsManager::requestResourceLock(const ResourceID &resourceID)
{
    if( std::find( selectableTextureWalls_.begin(), selectableTextureWalls_.end(), resourceID ) == selectableTextureWalls_.end() ){
        throw std::runtime_error( "Texture wall not selectable" );
    }

    ResourcesOwnershipRequester::requestResourceLock( resourceID );
}


/***
 * 6. Shader communication
 ***/

void TextureWallsManager::sendTextureWallToShader( const ResourceID &resourceID ) const
{
    //textureWalls_.at( resourceID ).sendToShader( *this );
    // TODO: Send texture offset and scale.

    texturesManager_->sendTextureToShader( textureWalls_.at( resourceID ).textureID );
}


/***
 * 8. Resources ownership management
 ***/

void TextureWallsManager::lockResource(const ResourceID &resourceID, UserID newOwner)
{
    // TODO: Complete or delete.
    (void)( resourceID );
    (void)( newOwner );
}


void TextureWallsManager::unlockResourcesSelection(UserID currentOwner)
{
    // TODO: Complete or delete.
    (void)( currentOwner );
}


void TextureWallsManager::clearResourcesSelection(UserID currentOwner)
{
    // TODO: Complete or delete.
    (void)( currentOwner );
}


} // namespace como
