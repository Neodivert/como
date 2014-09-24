#include "texture_wall_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

TextureWallCommand::TextureWallCommand() :
    TypeCommand( CommandTarget::TEXTURE_WALL, TextureWallCommandType::TEXTURE_WALL_MODIFICATION, NO_USER ),
    textureWallID_( NO_RESOURCE )
{
    addPackable( &textureWallID_ );
}


TextureWallCommand::TextureWallCommand(const ResourceID &textureWallID, UserID userID, TextureWallCommandType commandType) :
    TypeCommand( CommandTarget::TEXTURE_WALL, commandType, userID ),
    textureWallID_( textureWallID )
{
    addPackable( &textureWallID_ );
}


TextureWallCommand::TextureWallCommand( const TextureWallCommand &b ) :
    TypeCommand( b ),
    textureWallID_( b.textureWallID_ )
{
    addPackable( &textureWallID_ );
}


/***
 * 3. Getters
 ***/

ResourceID TextureWallCommand::textureWallID() const
{
    return textureWallID_.getValue();
}

} // namespace como
