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

#include "texture_creation_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

TextureCreationCommand::TextureCreationCommand( const std::string &unpackingDirPath ) :
    TextureCommand( NO_RESOURCE, NO_USER, TextureCommandType::TEXTURE_CREATION ),
    textureFile_( unpackingDirPath )
{
    addPackable( &textureFile_ );
}


TextureCreationCommand::TextureCreationCommand( const ResourceID &textureID, const std::string& unpackingDirPath, const std::string textureFilePath ) :
    TextureCommand( textureID, textureID.getCreatorID(), TextureCommandType::TEXTURE_CREATION ),
    textureFile_( unpackingDirPath, textureFilePath )
{
    addPackable( &textureFile_ );
}


TextureCreationCommand::TextureCreationCommand( const TextureCreationCommand &b ) :
    TextureCommand( b ),
    textureFile_( b.textureFile_ )
{
    addPackable( &textureFile_ );
}


/***
 * 3. Getters
 ***/

std::string TextureCreationCommand::textureFilePath() const
{
    return textureFile_.getFilePath();
}

} // namespace como
