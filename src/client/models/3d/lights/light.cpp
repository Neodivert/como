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

Light::Light( LightType type, const char* meshPath, GLuint lightIndex, const std::uint8_t* color ) :
    Mesh( MeshType::LIGHT, meshPath ),
    type_( type )
{
    GLint currentShaderProgram = -1;
    char uniformName[64];

    // Get current shader program ID.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

    // Get the location of this light's color in the GLSL shader program.
    sprintf( uniformName, "lights[%u].color", lightIndex );
    colorLocation_ = glGetUniformLocation( currentShaderProgram, uniformName );

    // Update light color in the shader.
    setLightColor( color );

    checkOpenGL( "Light - Constructor end" );
}


/***
 * 3. Getters
 ***/

glm::vec3 Light::getLightColor() const
{
    GLint currentShaderProgram = -1;
    glm::vec3 color( 0.0f );

    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    glGetUniformfv( currentShaderProgram, colorLocation_, &color[0] );

    return color;
}


LightType Light::getType() const
{
    return type_;
}


/***
 * 4. Setters
 ***/

void Light::setLightColor( const std::uint8_t* color )
{
    // TODO: Use a vec4? (change in shader).
    glm::vec3 floatColor(
                color[0] / 255.0f,
                color[1] / 255.0f,
                color[2] / 255.0f
                );

    glUniform3fv( colorLocation_, 1, &floatColor[0] );
}

} // namespace como
