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
    LOCK
    return ( textureWalls_.at( textureWallID ).textureID != NO_RESOURCE );
}


ResourceHeadersList TextureWallsManager::getSelectableResourcesHeaders() const
{
    LOCK
    ResourceHeadersList headersList;

    for( const auto& textureWallID : selectableTextureWalls_ ){
        ResourceHeader currentHeader( textureWallID,
                                      textureWalls_.at( textureWallID ).name );
        headersList.push_back( currentHeader );
    }

    return headersList;
}


bool TextureWallsManager::isResourceSelectable( const ResourceID& resourceID ) const
{
    LOCK
    return ( std::find( selectableTextureWalls_.begin(),
                        selectableTextureWalls_.end(),
                        resourceID ) != selectableTextureWalls_.end() );
}


std::string TextureWallsManager::getResourceName( const ResourceID &resourceID ) const
{
    LOCK
    return textureWalls_.at( resourceID ).name;
}


TextureWallHandler *TextureWallsManager::getCurrentTextureWall() const
{
    LOCK
    return currentTextureWallHandler_.get();
}


/***
 * 4. Local texture walls management
 ***/

ResourceID TextureWallsManager::createTextureWall( std::string name, const ResourceID& meshID )
{
    LOCK

    ResourceID textureWallID = reserveResourceIDs( 1 );

    createTextureWall( name, textureWallID, meshID );

    return textureWallID;
}


void TextureWallsManager::createTextureWall(std::string name, const ResourceID &textureWallID, const ResourceID &meshID)
{
    LOCK

    textureWalls_.emplace( textureWallID, name );
    meshesTextureWalls_[meshID].push_back( textureWallID );

    toggleTextureWallSeletable( textureWallID, true );

    notifyElementInsertion( textureWallID );
}


void TextureWallsManager::toggleTextureWallSeletable( const ResourceID& textureWallID, bool selectable )
{
    LOCK
    if( selectable ){
        selectableTextureWalls_.push_back( textureWallID );
    }else{
        selectableTextureWalls_.remove( textureWallID );
    }
    notifyElementUpdate( textureWallID );
}


void TextureWallsManager::toggleMeshTextureWallsSeletable( const ResourceID& meshID, bool selectable )
{
    LOCK

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
    LOCK

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
    LOCK

    for( const ResourceID& textureWallID : selectableTextureWalls_ ){
        notifyElementDeletion( textureWallID );
        textureWalls_.erase( textureWallID );
    }

    selectableTextureWalls_.clear();
}


TextureWallHandler* TextureWallsManager::selectTextureWall( const ResourceID &textureWallID )
{
    LOCK

    currentTextureWallHandler_ = std::unique_ptr< TextureWallHandler >(
                new TextureWallHandler( server(), textureWallID, textureWalls_.at( textureWallID ) ) );
    return currentTextureWallHandler_.get();
}


bool TextureWallsManager::thereIsSelectableTextureWalls() const
{
    LOCK

    return ( selectableTextureWalls_.size() > 0 );
}


/***
 * 5. Resources ownership requesting
 ***/

void TextureWallsManager::requestResourceLock( const ResourceID &resourceID )
{
    LOCK

    if( !isResourceSelectable( resourceID ) ){
        throw std::runtime_error( "Texture wall not selectable" );
    }

    ResourcesOwnershipRequester::requestResourceLock( resourceID );
}


/***
 * 6. Remote command execution
 ***/

void TextureWallsManager::executeRemoteCommand(const TextureWallCommand &command)
{
    LOCK

    TextureWall& textureWall = textureWalls_.at( command.textureWallID() );

    switch( command.getType() ){
        case TextureWallCommandType::TEXTURE_CHANGE:{
            const TextureWallTextureChangeCommand &textureChangeCommand =
                    dynamic_cast< const TextureWallTextureChangeCommand& >( command );
            textureWall.textureID =
                    textureChangeCommand.textureID();

            notifyElementUpdate( textureChangeCommand.textureWallID() );
        }break;
        case TextureWallCommandType::TEXTURE_WALL_MODIFICATION:
            const TextureWallModificationCommand& modificationCommand =
                    dynamic_cast< const TextureWallModificationCommand& >( command );

            switch( modificationCommand.parameterName() ){
                case TextureWallParameterName::TEXTURE_OFFSET_X:
                    textureWall.textureOffset.x = modificationCommand.parameterNewValue();
                break;
                case TextureWallParameterName::TEXTURE_OFFSET_Y:
                    textureWall.textureOffset.y = modificationCommand.parameterNewValue();
                break;
                case TextureWallParameterName::TEXTURE_SCALE_X:
                    textureWall.textureScale.x = modificationCommand.parameterNewValue();
                break;
                case TextureWallParameterName::TEXTURE_SCALE_Y:
                    textureWall.textureScale.y = modificationCommand.parameterNewValue();
                break;
            }

            notifyElementUpdate( modificationCommand.textureWallID() );
        break;
    }
}


/***
 * 7. Shader communication
 ***/

void TextureWallsManager::sendTextureWallToShader( const ResourceID &resourceID ) const
{
    LOCK

    OpenGL::checkStatus( "TextureWallsManager::sendTextureWallToShader - begin" );
    texturesManager_->sendTextureToShader( textureWalls_.at( resourceID ).textureID,
                                           textureWalls_.at( resourceID ).textureOffset,
                                           textureWalls_.at( resourceID ).textureScale );
    OpenGL::checkStatus( "TextureWallsManager::sendTextureWallToShader - end" );
}


/***
 * 8. Resources ownership management
 ***/

void TextureWallsManager::lockResource(const ResourceID &resourceID, UserID newOwner)
{
    LOCK
    // TODO: Complete or delete.
    (void)( resourceID );
    (void)( newOwner );
}


void TextureWallsManager::unlockResourcesSelection(UserID currentOwner)
{
    LOCK
    // TODO: Complete or delete.
    (void)( currentOwner );
}


void TextureWallsManager::clearResourcesSelection(UserID currentOwner)
{
    LOCK
    // TODO: Complete or delete.
    (void)( currentOwner );
}


} // namespace como
