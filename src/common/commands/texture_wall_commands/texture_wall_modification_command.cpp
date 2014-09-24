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

#include "texture_wall_modification_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

TextureWallModificationCommand::TextureWallModificationCommand() :
    TextureWallCommand( NO_RESOURCE, NO_USER, TextureWallCommandType::TEXTURE_WALL_MODIFICATION ),
    parameterName_( TextureWallParameterName::TEXTURE_OFFSET_X ),
    newValue_( 0.0f )
{
    addPackable( &parameterName_ );
    addPackable( &newValue_ );
}


TextureWallModificationCommand::TextureWallModificationCommand( const ResourceID &textureWallID,
                                                                UserID userID,
                                                                TextureWallParameterName parameterName,
                                                                float newValue ) :
    TextureWallCommand( textureWallID, userID, TextureWallCommandType::TEXTURE_WALL_MODIFICATION ),
    parameterName_( parameterName ),
    newValue_( newValue )
{
    addPackable( &parameterName_ );
    addPackable( &newValue_ );
}


TextureWallModificationCommand::TextureWallModificationCommand( const TextureWallModificationCommand& b ) :
    TextureWallCommand( b ),
    parameterName_( b.parameterName_ ),
    newValue_( b.newValue_ )
{
    addPackable( &parameterName_ );
    addPackable( &newValue_ );
}


/***
 * 3. Getters
 ***/

TextureWallParameterName TextureWallModificationCommand::parameterName() const
{
    return parameterName_.getValue();
}


float TextureWallModificationCommand::parameterNewValue() const
{
    return newValue_.getValue();
}


} // namespace como
