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

#include "texture_wall_texture_change_command.hpp"

namespace como {


/***
 * 1. Construction
 ***/

TextureWallTextureChangeCommand::TextureWallTextureChangeCommand() :
    TextureWallCommand( NO_RESOURCE, NO_USER, TextureWallCommandType::TEXTURE_CHANGE ),
    textureID_( NO_RESOURCE )
{
    addPackable( &textureID_ );
}


TextureWallTextureChangeCommand::TextureWallTextureChangeCommand(
        const ResourceID &textureWallID,
        UserID userID,
        const ResourceID &newTextureID ) :
    TextureWallCommand( textureWallID, userID, TextureWallCommandType::TEXTURE_CHANGE ),
    textureID_( newTextureID )
{
    addPackable( &textureID_ );
}


TextureWallTextureChangeCommand::TextureWallTextureChangeCommand( const TextureWallTextureChangeCommand &b ) :
    TextureWallCommand( b ),
    textureID_( b.textureID_ )
{
    addPackable( &textureID_ );
}


/***
 * 3. Getters
 ***/

ResourceID TextureWallTextureChangeCommand::textureID() const
{
    return textureID_.getValue();
}

} // namespace como
