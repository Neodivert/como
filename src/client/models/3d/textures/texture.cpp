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
#include <cassert>
#include <SDL2/SDL_image.h>

namespace como {

/***
 * 1. Construction
 ***/

Texture::Texture( const TextureInfo& textureInfo )
{
    SDL_Surface* textureImage = nullptr;
    SDL_RWops* textureData = nullptr;

    // Generate a texture GL name.
    glGenTextures( 1, &oglName_ );
    assert( oglName_ != 0 );

    // Bind the previous texture as a 2D texture.
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, oglName_ );

    // Load texture data from memory.
    textureData = SDL_RWFromConstMem(
                &( textureInfo.imageFileData[0] ),
                textureInfo.imageFileData.size() );

    textureImage = IMG_Load_RW( textureData, 0 );
    if( !textureImage ){
        throw std::runtime_error( IMG_GetError() );
    }

    // Set texture storage and data.
    glTexImage2D(
                GL_TEXTURE_2D,
                1,
                GL_RGBA, // TODO: Retrieve this value from textureImage.
                textureImage->w,
                textureImage->h,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                textureImage->pixels // TODO: Do I have to have the ->pitch into account?
                );

    // Free resources.
    SDL_FreeSurface( textureImage );

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
