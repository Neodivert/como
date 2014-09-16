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

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

extern "C" {
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
}

#include <memory>
#include <common/primitives/primitive_data/texture_info.hpp>

namespace como {

class Texture
{
    private:
        GLuint oglName_;
        GLint samplerShaderLocation_;

    public:
        /***
         * 1. Construction
         ***/
        Texture() = delete;
        Texture( const TextureInfo& textureInfo );
        Texture( const std::string& filePath );
        Texture( const Texture& ) = delete;
        Texture( Texture&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~Texture();


        /***
         * 3. Initialization
         ***/
    private:
        void initSamplerShaderLocation();
    public:


        /***
         * 4. Shader communication
         ***/
        void sendToShader() const;


        /***
         * 5. Operators
         ***/
        Texture& operator = ( const Texture& ) = delete;
        Texture& operator = ( Texture&& ) = delete;
};

typedef std::shared_ptr< Texture > TexturePtr;
typedef std::shared_ptr< const Texture > TextureConstPtr;

} // namespace como

#endif // TEXTURE_HPP
