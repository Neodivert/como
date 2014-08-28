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

#include "light.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace como {


/***
 * 1. Construction
 ***/

Light::Light( LightType type, GLuint index, const PackableColor& color, std::string path, MaterialConstPtr meshMaterial ) :
    Mesh( MeshType::LIGHT, path.c_str(), meshMaterial ),
    type_( type ),
    index_( index )
{
    GLint currentShaderProgram = -1;
    char uniformName[64];

    // Get current shader program ID.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

    // Get the location of this light's color in the GLSL shader program.
    sprintf( uniformName, "lights[%u].color", index_ );
    colorLocation_ = glGetUniformLocation( currentShaderProgram, uniformName );
    assert( colorLocation_ != -1 );

    // Get the location of this light's ambient coefficient in the GLSL shader
    // program.
    sprintf( uniformName, "lights[%u].ambientCoefficient", index_ );
    ambientCoefficientLocation_ = glGetUniformLocation( currentShaderProgram, uniformName );
    assert( ambientCoefficientLocation_ != -1 );

    // Update light color in the shader.
    setLightColor( color );
    setAmbientCoefficient( 0.01f );
}


/***
 * 3. Getters
 ***/

LightType Light::getLightType() const
{
    return type_;
}

PackableColor Light::getLightColor() const
{
    GLint currentShaderProgram = -1;
    glm::vec3 color( 0.0f );

    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    glGetUniformfv( currentShaderProgram, colorLocation_, &color[0] );

    return PackableColor( color );
}


float Light::getAmbientCoefficient() const
{
    GLint currentShaderProgram = -1;
    float ambientCoefficient;

    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    glGetUniformfv( currentShaderProgram, ambientCoefficientLocation_, &ambientCoefficient );

    return ambientCoefficient;
}


GLuint Light::getBaseLightIndex() const
{
    return index_;
}


/***
 * 4. Setters
 ***/

void Light::setLightColor( const PackableColor &color )
{
    glUniform3fv( colorLocation_, 1, glm::value_ptr( color.toVec3() ) );
}


void Light::setAmbientCoefficient( float coefficient )
{
    glUniform1f( ambientCoefficientLocation_, coefficient );
}

} // namespace como
