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

namespace como {

/***
 * 1. Construction
 ***/

Texture::Texture( const std::string &imagePath )
{
    (void)( imagePath ); // TODO: Remove.

    // Generate a new texture object.
    glGenTextures( 1, &oglName_ );

    // Bind the texture object to a texture unit and target.
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, oglName_ );

    // Retrieve the location in shader of the texture sampler for futher
    // access.
    initSamplerShaderLocation();

    // Load texture data from file.
    loadFromFile( imagePath );
}


/***
 * 2. Destruction
 ***/

Texture::~Texture()
{
    // TODO: Release Texture name (OpenGL).
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


void Texture::loadFromFile( const std::string& imagePath )
{
    (void)( imagePath ); // TODO: Remove
    int imageWidth = 32; // TODO: Retrieve dimensions from image.
    int imageHeight = 32;

    unsigned char* imageData = SOIL_load_image( imagePath.c_str(), &imageWidth, &imageHeight, nullptr, 0 );

    // Set the texture's storage.
    glTexStorage2D( GL_TEXTURE_2D,  // target
                    1,              // levels
                    GL_RGB8,       // internal format
                    imageWidth,
                    imageHeight );

    // Set the texture's data.
    glTexSubImage2D( GL_TEXTURE_2D,
                     0,
                     0,
                     0,
                     imageWidth,
                     imageHeight,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     imageData );
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
