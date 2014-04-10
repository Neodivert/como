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

Light::Light( glm::vec3 position, glm::vec3 lightColor ) :
    Mesh( "data/primitives/system/camera.obj" ) // TODO: Create a Light primitive. And use a constant.
{
    setPosition( position );
    setLightColor( lightColor );
}


/***
 * 3. Getters
 ***/

glm::vec3 Light::getPosition()
{
    return position_;
}


glm::vec3 Light::getLightColor()
{
    return lightColor_;
}


/***
 * 4. Setters
 ***/

void Light::setPosition( const glm::vec3& position )
{
    position_ = position;
}


void Light::setLightColor( const glm::vec3& lightColor )
{
    lightColor_ = lightColor;
}

/*! \brief Set Ambient light */

void Light::setAmbientLight( const glm::vec3& ambientLight )
{
    GLint currentShaderProgram = -1;
    GLint uniformLocation = -1;

    // Get current shader program id.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    checkOpenGL( "Light::setAmbientLight - After getting current shader program ID");

    // Get location of uniform shader variable "color".
    uniformLocation = glGetUniformLocation( currentShaderProgram, "ambientLight" );
    checkOpenGL( "Light::setAmbientLight - After getting location of shader uniform \"ambientLight\"" );

    // Feed uniform shader variable "ambientLight" with given ambient light
    // argument.
    glUniform3fv( uniformLocation, 1, &ambientLight[0] );
}

} // namespace como
