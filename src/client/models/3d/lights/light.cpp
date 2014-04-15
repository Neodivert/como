/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "light.hpp"

namespace como {

/***
 * 1. Construction
 ***/

Light::Light( const char* meshPath, GLuint lightIndex, glm::vec3 position, glm::vec3 color ) :
    Mesh( meshPath )
{
    GLint currentShaderProgram = -1;
    char uniformName[64];

    // Get current shader program ID.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

    std::cout << "currentShaderProgram: " << currentShaderProgram << std::endl;

    // Get the location of this light's position in the GLSL shader program.
    sprintf( uniformName, "lights[%u].position", lightIndex );
    positionLocation_ = glGetUniformLocation( currentShaderProgram, uniformName );

    // Get the location of this light's color in the GLSL shader program.
    sprintf( uniformName, "lights[%u].color", lightIndex );
    colorLocation_ = glGetUniformLocation( currentShaderProgram, uniformName );

    // Update light's position and color in the shader.
    setPosition( position );
    setLightColor( color );

    checkOpenGL( "Light - Constructor end" );
}


/***
 * 3. Getters
 ***/

glm::vec3 Light::getPosition()
{
    GLint currentShaderProgram = -1;
    glm::vec3 position( 0.0f );

    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    glGetUniformfv( currentShaderProgram, positionLocation_, &position[0] );

    return position;
}


glm::vec3 Light::getLightColor()
{
    GLint currentShaderProgram = -1;
    glm::vec3 color( 0.0f );

    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    glGetUniformfv( currentShaderProgram, colorLocation_, &color[0] );

    return color;
}


/***
 * 4. Setters
 ***/

void Light::setPosition( const glm::vec3& position )
{
    glUniform3fv( positionLocation_, 1, &position[0] );
}


void Light::setLightColor( const glm::vec3& color )
{
    glUniform3fv( colorLocation_, 1, &color[0] );
}

} // namespace como
