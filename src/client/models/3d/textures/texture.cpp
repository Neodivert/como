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
extern "C" {
    #include <SDL2/SDL_image.h>
}
#include <client/models/utilities/open_gl.hpp>

namespace como {

/***
 * 1. Construction
 ***/

Texture::Texture( const TextureInfo& textureInfo )
{
    SDL_Surface* textureImage = nullptr;
    SDL_RWops* textureData = nullptr;
    GLint textureInternalFormat = GL_RGBA8;
    GLint textureFormat;

    // Generate a texture GL name.
    glGenTextures( 1, &oglName_ );
    assert( oglName_ != 0 );

    // Bind the previous texture as a 2D texture.
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, oglName_ );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    //glPixelStorei( GL_UNPACK_ROW_LENGTH, 2 );
    //glPixelStorei( GL_UNPACK_IMAGE_HEIGHT, 2 );
    //glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    // Load texture data from memory.
    textureData = SDL_RWFromConstMem(
                &( textureInfo.imageFileData[0] ),
                textureInfo.imageFileData.size() );

    textureImage = IMG_Load_RW( textureData, 0 );
    if( !textureImage ){
        // TODO: Do I have to consider any other case?
        textureImage = IMG_LoadTGA_RW( textureData );
        if( !textureImage ){
            throw std::runtime_error( IMG_GetError() );
        }
    }

    width_ = textureImage->w;
    height_ = textureImage->h;

    // TODO: Take components order into account too (RGBA != ABGR).
    if( textureImage->format->BytesPerPixel == 4 ){
        textureInternalFormat = GL_RGBA8;
        textureFormat = GL_RGBA;
    }else if( textureImage->format->BytesPerPixel == 3 ){
        textureInternalFormat = GL_RGB8;
        textureFormat = GL_RGB;
    }else{
        throw std::runtime_error( "Unexpected number of Bytes Per Pixel in texture (" +
                                  std::to_string( textureImage->format->BytesPerPixel ) +
                                  ")" );
    }

    OpenGL::checkStatus( "Before Texture::glTextImage2D()" );

    // Set texture storage and data.
    glTexImage2D(
                GL_TEXTURE_2D,
                0,
                textureInternalFormat,
                textureImage->w,
                textureImage->h,
                0,
                textureFormat,
                GL_UNSIGNED_BYTE,
                textureImage->pixels // TODO: Do I have to have the ->pitch into account?
                );

    OpenGL::checkStatus( "After Texture::glTextImage2D()" );

    // Free resources.
    SDL_FreeRW( textureData );
    SDL_FreeSurface( textureImage );

    // Retrieve the location in shader of the texture sampler for futher
    // access.
    initSamplerShaderLocation();
}

Texture::Texture( const string& filePath ) :
    Texture( TextureInfo( filePath ) )
{}


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
    samplerShaderLocation_ = glGetUniformLocation( currentShaderProgram, "textureSampler" );
    assert( samplerShaderLocation_ != -1 );
}


/***
 * 4. Shader communication
 ***/

void Texture::sendToShader() const
{
    // Connect sampler to texture unit 0.
    glActiveTexture( GL_TEXTURE0 );
    glUniform1i( samplerShaderLocation_, 0 );
    glBindTexture( GL_TEXTURE_2D, oglName_ );
}

}
