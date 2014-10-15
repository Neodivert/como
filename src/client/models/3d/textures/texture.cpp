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

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

namespace como {

/***
 * 1. Construction
 ***/

Texture::Texture( const ResourceID& id, const std::string& name, const TextureInfo& textureInfo )
{
    SDL_Surface* textureImage = nullptr;
    SDL_RWops* textureData = nullptr;
    GLint textureInternalFormat = GL_RGBA8;

    // Generate a texture GL name.
    glGenTextures( 1, &oglName_ );
    assert( oglName_ != 0 );

    // Bind the previous texture as a 2D texture.
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, oglName_ );

    // TODO: Change to GL_LINEAR?
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
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

    // TODO: Take components order into account too (RGBA != ABGR).
    if( textureImage->format->BytesPerPixel == 4 ){
        textureInternalFormat = GL_RGBA8;
        format_ = GL_RGBA;
    }else if( textureImage->format->BytesPerPixel == 3 ){
        textureInternalFormat = GL_RGB8;
        format_ = GL_RGB;
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
                format_,
                GL_UNSIGNED_BYTE,
                textureImage->pixels // TODO: Do I have to have the ->pitch into account?
                );

    OpenGL::checkStatus( "After Texture::glTextImage2D()" );

    const unsigned int componentsPerPixel = ( format_ == GL_RGBA ) ? 4 : 3;
    const unsigned int pixelDataSize =
            componentsPerPixel *
            textureImage->w *
            textureImage->h;

    // Fill texture data.
    data_.id = id;
    data_.name = name;
    data_.width = textureImage->w;
    data_.height = textureImage->h;
    data_.format = format_;
    data_.pixels.resize( pixelDataSize );
    memcpy( data_.pixels.data(), textureImage->pixels, pixelDataSize );

    // Free resources.
    SDL_FreeRW( textureData );
    SDL_FreeSurface( textureImage );

    // Retrieve the location in shader of the texture sampler for futher
    // access.
    initSamplerShaderLocation();

    OpenGL::checkStatus( "Texture constructor - end" );
}

Texture::Texture( const ResourceID& id, const std::string& filePath ) :
    Texture( id,
             boost::filesystem::basename( filePath ) +
             boost::filesystem::extension( filePath ),
             TextureInfo( filePath ) )
{}


/***
 * 2. Destruction
 ***/

Texture::~Texture()
{
    // TODO: If sometime move semantics are implemented, make sure
    // moved Texture doesn't delete OpenGL resources on
    // destruction.
    glDeleteTextures( 1, &oglName_ );
}


/***
 * 3. Initialization
 ***/

void Texture::initSamplerShaderLocation()
{
    // TODO: Use OpenGL class.
    GLint currentShaderProgram;

    // Get current shader program id.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

    // Get location of sampler in shader.
    samplerShaderLocation_ = glGetUniformLocation( currentShaderProgram, "textureSampler" );
    assert( samplerShaderLocation_ != -1 );

    textureOffsetShaderLocation_ =
            glGetUniformLocation( currentShaderProgram, "textureOffset" );
    assert( textureOffsetShaderLocation_ != -1 );

    textureScaleShaderLocation_ =
            glGetUniformLocation( currentShaderProgram, "textureScale" );
    assert( textureScaleShaderLocation_ != -1 );
}


/***
 * 4. Getters
 ***/

TextureData Texture::data() const
{
    return data_;
}


/***
 * 5. Shader communication
 ***/

void Texture::sendToShader( glm::vec2 textureOffset, glm::vec2 textureScale ) const
{
    OpenGL::checkStatus( "Texture::sendToShader() - begin" );

    // Texture offset is expressed as a %, so we multiply it by 0.01 for
    // transforming it to range [0.0f, 1.0f]
    textureOffset *= 0.01f;

    // Send texture's offset and scale to shader.
    glUniform2fv( textureOffsetShaderLocation_,
                  1,
                  glm::value_ptr( textureOffset ) );
    glUniform2fv( textureScaleShaderLocation_,
                  1,
                  glm::value_ptr( textureScale ) );

    // Connect sampler to texture unit 0.
    glActiveTexture( GL_TEXTURE0 );
    glUniform1i( samplerShaderLocation_, 0 );
    glBindTexture( GL_TEXTURE_2D, oglName_ );
    OpenGL::checkStatus( "Texture::sendToShader() - end" );
}

}
