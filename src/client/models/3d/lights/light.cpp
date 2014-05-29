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

Light::Light( LightType type, const char* meshPath, GLuint lightIndex, MaterialConstPtr material, PackableColor lightColor ) :
    Mesh( MeshType::LIGHT, meshPath, material ),
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
    setLightColor( lightColor );

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

void Light::setLightColor( PackableColor color )
{
    glUniform3fv( colorLocation_, 1, &color.toVec3()[0] );
}

} // namespace como
