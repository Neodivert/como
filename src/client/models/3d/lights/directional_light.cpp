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

#include "directional_light.hpp"

namespace como {

/***
 * 1. Construction
 ***/

DirectionalLight::DirectionalLight( glm::vec3 lightColor ) :
    Light( "data/primitives/system/directional_light.obj", 0, lightColor )
{
    GLint currentShaderProgram = 0;
    const GLuint lightIndex = 0; // TODO: Retrieve this as an method argument.
    char uniformName[64];
    GLint lightIndexLocation = -1;

    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

    // Get the location of the DirectionalLight::lightIndex variable in shader.
    sprintf( uniformName, "directionalLight.lightIndex" );
    lightIndexLocation = glGetUniformLocation( currentShaderProgram, uniformName );

    // Get the location of the DirectionalLight::lightVector variable in shader.
    sprintf( uniformName, "directionalLight.lightVector" );
    lightVectorLocation_ = glGetUniformLocation( currentShaderProgram, uniformName );

    // Get the location of the DirectionalLight::halfVector variable in shader.
    sprintf( uniformName, "directionalLight.halfVector" );
    halfVectorLocation_ = glGetUniformLocation( currentShaderProgram, uniformName );

    // Set the light index in shader.
    glUniform1i( lightIndexLocation, lightIndex );
}

/***
 * 3. Setters
 ***/

void DirectionalLight::updateOrientationInShader()
{
    // Compute transformed light vector.
    // TODO: Extract original light vector from DirectionalLight primitive?
    glm::vec3 lightVector = glm::vec3( transformationMatrix * glm::vec4( 0.0f, -1.0f, 0.0f, 1.0f ) );

    // Update lightVector in shader.
    glUniform4fv( lightVectorLocation_, 1, &lightVector[0] );
}


} // namespace como
