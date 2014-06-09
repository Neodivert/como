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

#include "light_properties.hpp"

namespace como {


/***
 * 1. Construction
 ***/

LightProperties::LightProperties( LightType type, GLuint index, const PackableColor& color ) :
    type_( type )
{
    GLint currentShaderProgram = -1;
    char uniformName[64];

    // Get current shader program ID.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

    // Get the location of this light's color in the GLSL shader program.
    sprintf( uniformName, "lights[%u].color", index );
    colorLocation_ = glGetUniformLocation( currentShaderProgram, uniformName );

    std::cout << "MeshLight color location (" << uniformName << "): " << colorLocation_ << std::endl;

    // Update light color in the shader.
    setLightColor( color );

    checkOpenGL( "MeshLight - Constructor end" );
}


/***
 * 3. Getters
 ***/

PackableColor LightProperties::getLightColor() const
{
    GLint currentShaderProgram = -1;
    glm::vec3 color( 0.0f );

    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    glGetUniformfv( currentShaderProgram, colorLocation_, &color[0] );

    return PackableColor( color );
}


/***
 * 4. Setters
 ***/

void LightProperties::setLightColor( const PackableColor &color )
{
    glUniform3fv( colorLocation_, 1, &color.toVec3()[0] );
}

} // namespace como
