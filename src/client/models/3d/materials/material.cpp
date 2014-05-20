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

#include "material.hpp"
#include <client/models/3d/drawable.hpp>


namespace como {

/***
 * 1. Construction
 ***/

Material::Material() :
    ambientReflexivity( 0.9f ),
    diffuseReflectivity( 0.9f ),
    specularReflectivity( 0.9f ),
    specularExponent( 0.9f )
{}


/***
 * 2. Shader comunication
 ***/

void Material::sendToShader() const
{
    GLint currentShaderProgram = -1;
    GLint uniformLocation = -1;

    // Get current shader program ID.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

    // Send ambient reflexivity to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.ambientReflexivity" );
    checkOpenGL( "Getting location of material.ambientReflexivity in shader" );
    glUniform3fv( uniformLocation, 1, &ambientReflexivity[0] );
    checkOpenGL( "Sending material.ambientReflexivity to shader" );

    // Send diffuse reflexivity to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.diffuseReflectivity" );
    checkOpenGL( "Getting location of material.diffuseReflectivity in shader" );
    glUniform3fv( uniformLocation, 1, &diffuseReflectivity[0] );
    checkOpenGL( "Sending material.diffuseReflectivity to shader" );

    // Send diffuse reflexivity to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.specularReflectivity" );
    checkOpenGL( "Getting location of material.specularReflectivity in shader" );
    glUniform3fv( uniformLocation, 1, &specularReflectivity[0] );
    checkOpenGL( "Sending material.specularReflectivity to shader" );

    // Send specular exponent to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.specularExponent" );
    checkOpenGL( "Getting location of material.specularExponent in shader" );
    glUniform1f( uniformLocation, specularExponent );
    checkOpenGL( "Sending material.specularExponent to shader" );
}

} // namespace como