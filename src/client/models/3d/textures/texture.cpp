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

#include "texture.hpp"
#include <SOIL/SOIL.h>
#include <cassert>

namespace como {

/***
 * 1. Construction
 ***/

Texture::Texture( const TextureInfo& textureInfo )
{
    oglName_ = SOIL_load_OGL_texture_from_memory(
                (const unsigned char*)( textureInfo.imageFileData.c_str() ),
                textureInfo.imageFileData.size(),
                0,
                0,
                SOIL_FLAG_TEXTURE_RECTANGLE );

    assert( glGetString( GL_EXTENSIONS ) != nullptr ); // TODO: Remove this in the future.
    assert( oglName_ != 0 );

    // Retrieve the location in shader of the texture sampler for futher
    // access.
    initSamplerShaderLocation();
}


/***
 * 2. Destruction
 ***/

Texture::~Texture()
{
    // TODO: Release Texture name (OpenGL).
    glDeleteTextures( 1, &oglName_ );
}


/***
 * 3. Initialization
 ***/

void Texture::initSamplerShaderLocation()
{
    GLint currentShaderProgram;

    // Get current shader program id.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

    // Get location of sampler in shader.
    samplerShaderLocation_ = glGetUniformLocation( currentShaderProgram, "sampler" );
}


/***
 * 4. Shader communication
 ***/

void Texture::sendToShader() const
{
    // Connect sampler to texture unit 0.
    glActiveTexture( GL_TEXTURE0 );
    glUniform1i( samplerShaderLocation_, 0 );
}


}
