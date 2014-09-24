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

#ifndef TEXTURE_WALL_COMMAND_HPP
#define TEXTURE_WALL_COMMAND_HPP

#include <common/commands/type_command.hpp>

namespace como {

enum class TextureWallCommandType : std::uint8_t {
    TEXTURE_CHANGE = 0,
    TEXTURE_WALL_MODIFICATION
};

class TextureWallCommand : public TypeCommand< TextureWallCommandType >
{
    public:
        /***
         * 1. Construction
         ***/
        TextureWallCommand();
        TextureWallCommand( const ResourceID& textureWallID,
                            UserID userID,
                            TextureWallCommandType commandType );
        TextureWallCommand( const TextureWallCommand& );
        TextureWallCommand( TextureWallCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~TextureWallCommand() = default;


        /***
         * 3. Getters
         ***/
        ResourceID textureWallID() const;


        /***
         * 4. Operators
         ***/
        TextureWallCommand& operator = ( const TextureWallCommand& ) = delete;
        TextureWallCommand& operator = ( TextureWallCommand&& ) = delete;

    private:
        PackableResourceID textureWallID_;
};

} // namespace como

#endif // TEXTURE_WALL_COMMAND_HPP
