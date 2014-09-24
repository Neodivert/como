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

#ifndef TEXTURE_WALL_TEXTURE_CHANGE_COMMAND_HPP
#define TEXTURE_WALL_TEXTURE_CHANGE_COMMAND_HPP

#include "texture_wall_command.hpp"

namespace como {

class TextureWallTextureChangeCommand : public TextureWallCommand
{
    public:
        /***
         * 1. Construction
         ***/
        TextureWallTextureChangeCommand();
        TextureWallTextureChangeCommand( const ResourceID& textureWallID,
                                         UserID userID,
                                         const ResourceID& newTextureID );
        TextureWallTextureChangeCommand( const TextureWallTextureChangeCommand& );
        TextureWallTextureChangeCommand( TextureWallTextureChangeCommand&& ) = delete;
        COMMAND_CLONE_METHOD( TextureWallTextureChangeCommand )


        /***
         * 2. Destruction
         ***/
        virtual ~TextureWallTextureChangeCommand() = default;


        /***
         * 3. Getters
         ***/
        ResourceID textureID() const;


        /***
         * 4. Operators
         ***/
        TextureWallTextureChangeCommand& operator = ( const TextureWallTextureChangeCommand& ) = delete;
        TextureWallTextureChangeCommand& operator = ( TextureWallTextureChangeCommand&& ) = delete;


    private:
        PackableResourceID textureID_;
};

} // namespace como

#endif // TEXTURE_WALL_TEXTURE_CHANGE_COMMAND_HPP
