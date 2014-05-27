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
    color_( 1.0f, 1.0f, 1.0f, 1.0f ),
    ambientReflectivity_( 0.9f ),
    diffuseReflectivity_( 0.9f ),
    specularReflectivity_( 0.9f ),
    specularExponent_( 0.9f )
{}


Material::Material( PackableColor color ) :
    ambientReflectivity_( 0.9f ),
    diffuseReflectivity_( 0.9f ),
    specularReflectivity_( 0.9f ),
    specularExponent_( 0.9f )
{
    color_ = color.toVec4();
}


/***
 * 3. Getters
 ***/

PackableColor Material::getColor() const
{
    return PackableColor( color_ );
}

PackableColor Material::getAmbientReflectivity() const
{
    return PackableColor( ambientReflectivity_ );
}

PackableColor Material::getDiffuseReflectivity() const
{
    return PackableColor( diffuseReflectivity_ );
}

PackableColor Material::getSpecularReflectivity() const
{
    return PackableColor( specularReflectivity_ );
}

float Material::getSpecularExponent() const
{
    return specularExponent_;
}


/***
 * 4. Setters
 ***/

void Material::setColor( const PackableColor& color )
{
    color_ = color.toVec4();
}

void Material::setAmbientReflectivity( const PackableColor& ambientReflectivity )
{
    ambientReflectivity_ = ambientReflectivity.toVec3();
}

void Material::setDiffuseReflectivity( const PackableColor& diffuseReflectivity )
{
    diffuseReflectivity_ = diffuseReflectivity.toVec3();
}

void Material::setSpecularReflectivity( const PackableColor& specularReflectivity )
{
    specularReflectivity_ = specularReflectivity.toVec3();
}

void Material::setSpecularExponent( float specularExponent )
{
    specularExponent_ = specularExponent;
}


/***
 * 5. Shader comunication
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
    glUniform4fv( uniformLocation, 1, &color_[0] );
    checkOpenGL( "Sending material.color to shader" );

    // Send ambient reflectivity to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.ambientReflectivity" );
    checkOpenGL( "Getting location of material.ambientReflectivity in shader" );
    glUniform3fv( uniformLocation, 1, &ambientReflectivity_[0] );
    checkOpenGL( "Sending material.ambientReflectivity to shader" );

    // Send diffuse reflectivity to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.diffuseReflectivity" );
    checkOpenGL( "Getting location of material.diffuseReflectivity in shader" );
    glUniform3fv( uniformLocation, 1, &diffuseReflectivity_[0] );
    checkOpenGL( "Sending material.diffuseReflectivity to shader" );

    // Send diffuse reflectivity to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.specularReflectivity" );
    checkOpenGL( "Getting location of material.specularReflectivity in shader" );
    glUniform3fv( uniformLocation, 1, &specularReflectivity_[0] );
    checkOpenGL( "Sending material.specularReflectivity to shader" );

    // Send specular exponent to shader.
    uniformLocation = glGetUniformLocation( currentShaderProgram, "material.specularExponent" );
    checkOpenGL( "Getting location of material.specularExponent in shader" );
    glUniform1f( uniformLocation, specularExponent_ );
    checkOpenGL( "Sending material.specularExponent to shader" );
}


/***
 * 6. Auxiliar methods
 ***/

void Material::print() const
{
    std::cout << "Material" << std::endl
              << "----------------------------------------------" << std::endl
              << "Color: (" << color_[0] << ", " << color_[1] << ", " << color_[2] << ", " << color_[3] << ")" << std::endl
              << "Ambient reflectivity: (" << ambientReflectivity_[0] << ", " << ambientReflectivity_[1] << ", " << ambientReflectivity_[2] << ")" << std::endl
              << "Diffuse reflectivity: (" << diffuseReflectivity_[0] << ", " << diffuseReflectivity_[1] << ", " << diffuseReflectivity_[2] << ")" << std::endl
              << "Specular reflectivity: (" << specularReflectivity_[0] << ", " << specularReflectivity_[1] << ", " << specularReflectivity_[2] << ")" << std::endl
              << "Specular exponent: (" << specularExponent_ << ")" << std::endl
              << "----------------------------------------------" << std::endl;
}

} // namespace como
