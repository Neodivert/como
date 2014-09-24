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

#ifndef TEXTURE_COMMAND_HPP
#define TEXTURE_COMMAND_HPP

#include <common/commands/type_command.hpp>

namespace como {

enum class TextureCommandType : std::uint8_t {
    TEXTURE_CREATION = 0
};

class TextureCommand : public TypeCommand< TextureCommandType >
{
    public:
        /***
         * 1. Construction
         ***/
        TextureCommand() = delete;
        TextureCommand( const ResourceID& textureID, TextureCommandType commandType );
        TextureCommand( const TextureCommand& );
        TextureCommand( TextureCommand&& ) = delete;
        COMMAND_CLONE_METHOD( TextureCommand )


        /***
         * 2. Destruction
         ***/
        virtual ~TextureCommand() = default;


        /***
         * 3. Getters
         ***/
        ResourceID textureID() const;


        /***
         * 4. Operators
         ***/
        TextureCommand& operator = ( const TextureCommand& ) = delete;
        TextureCommand& operator = ( TextureCommand&& ) = delete;


    private:
        PackableResourceID textureID_;
};

} // namespace como

#endif // TEXTURE_COMMAND_HPP
