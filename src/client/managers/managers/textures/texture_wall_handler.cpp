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

#include "texture_wall_handler.hpp"

namespace como {


/***
 * 1. Construction
 ***/

TextureWallHandler::TextureWallHandler( ServerInterfacePtr server, const ResourceID& textureWallID, TextureWall& textureWall ) :
    ServerWriter( server ),
    textureWallID_( textureWallID ),
    textureWall_( &textureWall )
{}


/***
 * 3. Getters
 ***/

glm::vec2 TextureWallHandler::getTextureOffset() const
{
    return textureWall_->textureOffset;
}


glm::vec2 TextureWallHandler::getTextureScale() const
{
    return textureWall_->textureScale;
}


/***
 * 4. Setters
 ***/

void TextureWallHandler::setTextureID( const ResourceID& textureID )
{
    textureWall_->textureID = textureID;

    sendCommandToServer(
                CommandConstPtr(
                    new TextureWallTextureChangeCommand(
                        textureWallID_,
                        localUserID(),
                        textureID ) ) );
}


void TextureWallHandler::setTextureOffsetX( float newTextureOffsetX )
{
    textureWall_->textureOffset.x = newTextureOffsetX;

    sendCommandToServer(
                CommandConstPtr(
                    new TextureWallModificationCommand(
                        textureWallID_,
                        localUserID(),
                        TextureWallParameterName::TEXTURE_OFFSET_X,
                        newTextureOffsetX ) ) );
}


void TextureWallHandler::setTextureOffsetY( float newTextureOffsetY )
{
    textureWall_->textureOffset.y = newTextureOffsetY;

    sendCommandToServer(
                CommandConstPtr(
                    new TextureWallModificationCommand(
                        textureWallID_,
                        localUserID(),
                        TextureWallParameterName::TEXTURE_OFFSET_Y,
                        newTextureOffsetY ) ) );
}

} // namespace como
