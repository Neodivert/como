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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "texture_wall_sync_data.hpp"
#include <map>

namespace como {


/***
 * 1. Construction
 ***/

TextureWallSyncData::TextureWallSyncData( const Command *creationCommand, const ResourceID &id ) :
    ResourceSyncData( creationCommand, id ),
    textureWall_( "Unnamed" ) // TODO: Retrieve real name.
{}


/***
 * 3. Getters
 ***/

std::list<CommandConstPtr> TextureWallSyncData::generateUpdateCommands() const
{
    std::list<CommandConstPtr> updateCommands =
            ResourceSyncData::generateUpdateCommands();

    std::map< TextureWallParameterName, const float* > textureWallParametersMap =
    {
        { TextureWallParameterName::TEXTURE_OFFSET_X, &textureWall_.textureOffset.x },
        { TextureWallParameterName::TEXTURE_OFFSET_Y, &textureWall_.textureOffset.y },
        { TextureWallParameterName::TEXTURE_SCALE_X, &textureWall_.textureScale.x },
        { TextureWallParameterName::TEXTURE_SCALE_Y, &textureWall_.textureScale.y }
    };

    for( const auto& textureWallParameter : textureWallParametersMap ){
        updateCommands.push_back(
                    CommandConstPtr(
                        new TextureWallModificationCommand( resourceID(),
                                                            NO_USER,
                                                            textureWallParameter.first,
                                                            *(textureWallParameter.second) ) ) );
    }

    updateCommands.push_back(
                CommandConstPtr(
                    new TextureWallTextureChangeCommand(
                        resourceID(),
                        NO_USER,
                        textureWall_.textureID ) ) );

    return updateCommands;
}


/***
 * 4. Updating
 ***/

void TextureWallSyncData::processCommand( const Command &command )
{
    // FIXME: Duplicated code in TextureWallsManager
    const TextureWallCommand& textureWallCommand =
         dynamic_cast< const TextureWallCommand& >( command );

    switch( textureWallCommand.getType() ){
        case TextureWallCommandType::TEXTURE_CHANGE:{
            const TextureWallTextureChangeCommand &textureChangeCommand =
                    dynamic_cast< const TextureWallTextureChangeCommand& >( textureWallCommand );
            textureWall_.textureID =
                    textureChangeCommand.textureID();
        }break;
        case TextureWallCommandType::TEXTURE_WALL_MODIFICATION:
            const TextureWallModificationCommand& modificationCommand =
                    dynamic_cast< const TextureWallModificationCommand& >( textureWallCommand );

            switch( modificationCommand.parameterName() ){
                case TextureWallParameterName::TEXTURE_OFFSET_X:
                    textureWall_.textureOffset.x = modificationCommand.parameterNewValue();
                break;
                case TextureWallParameterName::TEXTURE_OFFSET_Y:
                    textureWall_.textureOffset.y = modificationCommand.parameterNewValue();
                break;
                case TextureWallParameterName::TEXTURE_SCALE_X:
                    textureWall_.textureScale.x = modificationCommand.parameterNewValue();
                break;
                case TextureWallParameterName::TEXTURE_SCALE_Y:
                    textureWall_.textureScale.y = modificationCommand.parameterNewValue();
                break;
            }
        break;
    }
}

} // namespace como
