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

#include "directional_light.hpp"

namespace como {

/***
 * 1. Construction
 ***/

DirectionalLight::DirectionalLight( GLuint directionalLightIndex, GLint lightIndex, const PackableColor& lightColor, const glm::vec3& lightVector, MaterialConstPtr meshMaterial ) :
    Light( LightType::DIRECTIONAL_LIGHT, lightIndex, lightColor, "data/system/primitives/directional_light.prim", meshMaterial ) // TODO: Load material from file.
{
    GLint currentShaderProgram = 0;
    char uniformName[64];

    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    assert( currentShaderProgram != 0 );

    // Get the location of the DirectionalLight::lightIndex variable in shader.
    sprintf( uniformName, "directionalLights[%u].lightIndex", directionalLightIndex );
    lightIndexLocation_ = glGetUniformLocation( currentShaderProgram, uniformName );
    assert( lightIndexLocation_ != -1 );

    // Get the location of the DirectionalLight::lightVector variable in shader.
    sprintf( uniformName, "directionalLights[%u].lightVector", directionalLightIndex );
    lightVectorLocation_ = glGetUniformLocation( currentShaderProgram, uniformName );
    assert( lightVectorLocation_ != -1 );

    // Get the location of the DirectionalLight::halfVector variable in shader.
    sprintf( uniformName, "directionalLights[%u].halfVector", directionalLightIndex );
    halfVectorLocation_ = glGetUniformLocation( currentShaderProgram, uniformName );

    // TODO: Replace comparison by "!= 1" when halfVectorLocation_ be implemented in shader.
    assert( halfVectorLocation_ == -1 );

    // Set the light index in shader.
    glUniform1i( lightIndexLocation_, lightIndex );

    // Initialize light vector in shader.
    setLightVector( lightVector );
    //setHalfVector( halfVector ); // TODO: Uncomment.
}


/***
 * 2. Destruction
 ***/

DirectionalLight::~DirectionalLight()
{
    // Disable this light in shader.
    glUniform1i( lightIndexLocation_, -1 );
}


/***
 * 3. Getters
 ***/

glm::vec3 DirectionalLight::getLightVector() const
{
    GLint currentShaderProgram = -1;
    glm::vec3 lightVector( 0.0f );

    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    glGetUniformfv( currentShaderProgram, lightVectorLocation_, &lightVector[0] );

    return lightVector;
}

glm::vec3 DirectionalLight::getHalfVector() const
{
    GLint currentShaderProgram = -1;
    glm::vec3 halfVector( 0.0f );

    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    glGetUniformfv( currentShaderProgram, halfVectorLocation_, &halfVector[0] );

    return halfVector;
}


GLint DirectionalLight::getLightIndex() const
{
    GLint currentShaderProgram = -1;
    GLint index = -1;

    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    glGetUniformiv( currentShaderProgram, lightIndexLocation_, &index );

    return index;
}


/***
 * 4. Setters
 ***/

void DirectionalLight::setLightVector( const glm::vec3& lightVector )
{
    glUniform3fv( lightVectorLocation_, 1, &lightVector[0] );

    // TODO: Update half vector.
}


/***
 * 3. Setters
 ***/

void DirectionalLight::update()
{
    // Update mesh associated to this light.
    Light::update();

    // Compute transformed light vector.
    setLightVector( glm::normalize( glm::vec3( modelMatrix_ * glm::vec4( 0.0f, -1.0f, 0.0f, 0.0f ) ) ) );
}


/***
 * 5. Lights management
 ***/

unsigned int DirectionalLight::getMaxLights()
{
    // TODO: Retrieve value from shader.
    return 4;
}


/***
 * 7. Auxiliar methods
 ***/

bool DirectionalLight::containsProperty( const void *property ) const
{
    return Mesh::containsProperty( property );
}

} // namespace como
