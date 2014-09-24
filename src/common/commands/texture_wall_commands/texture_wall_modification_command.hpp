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

#ifndef TEXTURE_WALL_MODIFICATION_COMMAND_HPP
#define TEXTURE_WALL_MODIFICATION_COMMAND_HPP

#include "texture_wall_command.hpp"
#include <common/packables/packable_float.hpp>

namespace como {

enum class TextureWallParameterName : std::uint8_t {
    TEXTURE_OFFSET_X = 0,
    TEXTURE_OFFSET_Y,
    TEXTURE_SCALE_X,
    TEXTURE_SCALE_Y
};

class TextureWallModificationCommand : public TextureWallCommand
{
    public:
        /***
         * 1. Construction
         ***/
        TextureWallModificationCommand();
        TextureWallModificationCommand( const ResourceID& textureWallID,
                                        TextureWallParameterName parameterName,
                                        float newValue );
        TextureWallModificationCommand( const TextureWallModificationCommand& );
        TextureWallModificationCommand( TextureWallModificationCommand&& ) = delete;
        COMMAND_CLONE_METHOD( TextureWallModificationCommand )


        /***
         * 2. Destruction
         ***/
        virtual ~TextureWallModificationCommand() = default;


        /***
         * 3. Getters
         ***/
        TextureWallParameterName parameterName() const;
        float parameterNewValue() const;


        /***
         * 4. Operators
         ***/
        TextureWallModificationCommand& operator = ( const TextureWallModificationCommand& ) = delete;
        TextureWallModificationCommand& operator = ( TextureWallModificationCommand&& ) = delete;


    private:
        PackableUint8< TextureWallParameterName > parameterName_;
        PackableFloat newValue_;
};

} // namespace como

#endif // TEXTURE_WALL_MODIFICATION_COMMAND_HPP
