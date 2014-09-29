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

#include <glm/glm.hpp>
#include <memory>
#include <common/primitives/primitive_data/texture_info.hpp>
#include <common/ids/resource_id.hpp>

namespace como {

struct TextureData {
    ResourceID id;
    std::string name;

    unsigned int width;
    unsigned int height;
    GLint format;

    std::vector< GLubyte > pixels;
};

class Texture
{
    private:
        GLuint oglName_;
        GLint format_;
        GLint samplerShaderLocation_;

        TextureData data_;

        GLint textureOffsetShaderLocation_;
        GLint textureScaleShaderLocation_;

    public:
        /***
         * 1. Construction
         ***/
        Texture() = delete;
        Texture( const ResourceID& id, const std::string& name, const TextureInfo& textureInfo );
        Texture( const ResourceID& id, const std::string& filePath );
        Texture( const Texture& ) = delete;
        Texture( Texture&& ) = delete;


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
         * 4. Getters
         ***/
        TextureData data() const;


        /***
         * 5. Shader communication
         ***/
        void sendToShader( glm::vec2 textureOffset = glm::vec2( 0.0f ),
                           glm::vec2 textureScale = glm::vec2( 1.0f ) ) const;


        /***
         * 6. Operators
         ***/
        Texture& operator = ( const Texture& ) = delete;
        Texture& operator = ( Texture&& ) = delete;
};

typedef std::unique_ptr< Texture > TexturePtr;
typedef std::unique_ptr< const Texture > TextureConstPtr;

} // namespace como

#endif // TEXTURE_HPP
