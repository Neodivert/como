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

#ifndef TEXTURE_CREATION_COMMAND_HPP
#define TEXTURE_CREATION_COMMAND_HPP

#include "texture_command.hpp"
#include <common/packables/packable_file.hpp>

namespace como {

class TextureCreationCommand : public TextureCommand
{
    public:
        /***
         * 1. Construction
         ***/
        TextureCreationCommand( const std::string& unpackingDirPath );
        TextureCreationCommand( const ResourceID& textureID, const std::string& unpackingDirPath, const std::string textureFilePath );
        TextureCreationCommand() = delete;
        TextureCreationCommand( const TextureCreationCommand& );
        TextureCreationCommand( TextureCreationCommand&& ) = delete;
        COMMAND_CLONE_METHOD( TextureCreationCommand )


        /***
         * 2. Destruction
         ***/
        virtual ~TextureCreationCommand() = default;


        /***
         * 3. Getters
         ***/
        std::string textureFilePath() const;


        /***
         * 4. Operators
         ***/
        TextureCreationCommand& operator = ( const TextureCreationCommand& ) = delete;
        TextureCreationCommand& operator = ( TextureCreationCommand&& ) = delete;


    private:
        PackableFile textureFile_;
};

} // namespace como

#endif // TEXTURE_CREATION_COMMAND_HPP
