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


ResourceHeadersList TextureWallsManager::getSelectableResourcesHeaders() const
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


bool TextureWallsManager::isResourceSelectable( const ResourceID& resourceID ) const
{
    return ( std::find( selectableTextureWalls_.begin(),
                        selectableTextureWalls_.end(),
                        resourceID ) != selectableTextureWalls_.end() );
}


string TextureWallsManager::getResourceName( const ResourceID &resourceID ) const
{
    return textureWalls_.at( resourceID ).name;
}


/***
 * 4. Local texture walls management
 ***/

ResourceID TextureWallsManager::createTextureWall( string name, const ResourceID& meshID )
{
    ResourceID textureWallID = reserveResourceIDs( 1 );

    textureWalls_.emplace( textureWallID, name );
    meshesTextureWalls_[meshID].push_back( textureWallID );

    toggleTextureWallSeletable( textureWallID, true );

    notifyElementInsertion( textureWallID );

    return textureWallID;
}


void TextureWallsManager::toggleTextureWallSeletable( const ResourceID& textureWallID, bool selectable )
{
    if( selectable ){
        selectableTextureWalls_.push_back( textureWallID );
    }else{
        selectableTextureWalls_.remove( textureWallID );
    }
    notifyElementUpdate( textureWallID );
}


void TextureWallsManager::toggleMeshTextureWallsSeletable( const ResourceID& meshID, bool selectable )
{
    if( meshesTextureWalls_.count( meshID ) == 0 ){
        return;
    }

    if( selectable ){
        for( const ResourceID& textureWallID : meshesTextureWalls_.at( meshID ) ){
            selectableTextureWalls_.push_back( textureWallID );
        }
    }else{
        for( const ResourceID& textureWallID : meshesTextureWalls_.at( meshID ) ){
            selectableTextureWalls_.remove( textureWallID );
        }
    }

    for( const ResourceID& textureWallID : meshesTextureWalls_.at( meshID ) ){
        notifyElementUpdate( textureWallID );
    }
}


void TextureWallsManager::unlockSelectableTextureWalls()
{
    std::list< ResourceID > removedElementIDs;

    for( const ResourceID& textureWallID : selectableTextureWalls_ ){
        removedElementIDs.push_back( textureWallID );
    }

    selectableTextureWalls_.clear();

    for( const ResourceID& textureWallID : removedElementIDs ){
        notifyElementUpdate( textureWallID );
    }
}


void TextureWallsManager::removeSelectableTextureWalls()
{
    for( const ResourceID& textureWallID : selectableTextureWalls_ ){
        notifyElementDeletion( textureWallID );
        textureWalls_.erase( textureWallID );
    }

    selectableTextureWalls_.clear();
}


TextureWallHandler *TextureWallsManager::selectTextureWall( const ResourceID &textureWallID )
{
    currentTextureWallHandler_ = std::unique_ptr< TextureWallHandler >(
                new TextureWallHandler( server(), textureWallID, textureWalls_.at( textureWallID ) ) );
    return currentTextureWallHandler_.get();
}


/***
 * 5. Resources ownership requesting
 ***/

void TextureWallsManager::requestResourceLock( const ResourceID &resourceID )
{
    if( !isResourceSelectable( resourceID ) ){
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
