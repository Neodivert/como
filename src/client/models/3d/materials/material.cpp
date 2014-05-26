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
    color( 1.0f, 1.0f, 1.0f, 1.0f ),
    ambientReflectivity( 0.9f ),
    diffuseReflectivity( 0.9f ),
    specularReflectivity( 0.9f ),
    specularExponent( 0.9f )
{}


Material::Material( PackableColor color ) :
    ambientReflectivity( 0.9f ),
    diffuseReflectivity( 0.9f ),
    specularReflectivity( 0.9f ),
    specularExponent( 0.9f )
{
    this->color = color.toVec4();
}


/***
 * 2. Shader comunication
 ***/

void Material::sendToShader() const
{
    GLint currentShaderProgram = -1;
    GLint uniformLocation = -1;

    // Get current shader program ID.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

    // Send material color to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.color" );
    checkOpenGL( "Getting location of material.color in shader" );
    glUniform4fv( uniformLocation, 1, &color[0] );
    checkOpenGL( "Sending material.color to shader" );

    // Send ambient reflectivity to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.ambientReflectivity" );
    checkOpenGL( "Getting location of material.ambientReflectivity in shader" );
    glUniform3fv( uniformLocation, 1, &ambientReflectivity[0] );
    checkOpenGL( "Sending material.ambientReflectivity to shader" );

    // Send diffuse reflectivity to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.diffuseReflectivity" );
    checkOpenGL( "Getting location of material.diffuseReflectivity in shader" );
    glUniform3fv( uniformLocation, 1, &diffuseReflectivity[0] );
    checkOpenGL( "Sending material.diffuseReflectivity to shader" );

    // Send diffuse reflectivity to shader.
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


void Material::print() const
{
    std::cout << "Material" << std::endl
              << "----------------------------------------------" << std::endl
              << "Color: (" << color[0] << ", " << color[1] << ", " << color[2] << ", " << color[3] << ")" << std::endl
              << "Ambient reflectivity: (" << ambientReflectivity[0] << ", " << ambientReflectivity[1] << ", " << ambientReflectivity[2] << ")" << std::endl
              << "Diffuse reflectivity: (" << diffuseReflectivity[0] << ", " << diffuseReflectivity[1] << ", " << diffuseReflectivity[2] << ")" << std::endl
              << "Specular reflectivity: (" << specularReflectivity[0] << ", " << specularReflectivity[1] << ", " << specularReflectivity[2] << ")" << std::endl
              << "Specular exponent: (" << specularExponent << ")" << std::endl
              << "----------------------------------------------" << std::endl;
}

} // namespace como
