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

Light::Light( LightType type, const PackableColor& color, std::string path, OpenGL& openGL ) :
    ImportedMesh( path ),
    type_( type ),
    index_( lockShaderLight( openGL ) )
{
    const GLint defaultShaderProgram = openGL.getShaderProgramID( ShaderProgramType::DEFAULT );
    char uniformName[64];

    // Get the location of this light's isValid in the GLSL shader program.
    sprintf( uniformName, "lights[%u].isValid", index_ );
    isValidLocation_ = glGetUniformLocation( defaultShaderProgram, uniformName );
    assert( isValidLocation_ != -1 );

    // Get the location of this light's color in the GLSL shader program.
    sprintf( uniformName, "lights[%u].color", index_ );
    colorLocation_ = glGetUniformLocation( defaultShaderProgram, uniformName );
    assert( colorLocation_ != -1 );

    // Get the location of this light's ambient coefficient in the GLSL shader
    // program.
    sprintf( uniformName, "lights[%u].ambientCoefficient", index_ );
    ambientCoefficientLocation_ = glGetUniformLocation( defaultShaderProgram, uniformName );
    assert( ambientCoefficientLocation_ != -1 );

    // Update light color in the shader.
    setLightColor( color );
    setAmbientCoefficient( 0.01f );
}


/***
 * 2. Destruction
 ***/

Light::~Light()
{
    // Disable this light in shader.
    glUniform1i( isValidLocation_, false );
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
    return PackableColor( color_ );
}


float Light::getAmbientCoefficient() const
{
    return ambientCoefficient_;
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
    color_ = color.toVec3();
}


void Light::setAmbientCoefficient( float coefficient )
{
    ambientCoefficient_ = coefficient;
}


/***
 * 5. Shader communication
 ***/

void Light::sendToShader( OpenGL &openGL, const glm::mat4& viewMatrix ) const
{
    (void)( openGL );
    (void)( viewMatrix );

    glUniform3fv( colorLocation_, 1, glm::value_ptr( color_ ) );
    glUniform1f( ambientCoefficientLocation_, ambientCoefficient_ );
}


/***
 * 7. Auxiliar methods
 ***/

// FIXME: Duplicated code.
GLuint Light::lockShaderLight( OpenGL& openGL )
{
    char uniformName[64] = {0};
    GLint varLocation = -1;
    unsigned int currentLightIndex;

    // Get current shader program ID.

    for( currentLightIndex = 0; currentLightIndex < 4; currentLightIndex++ ){ // Retrieve MAX_LIGHTS from shader.
        sprintf( uniformName, "lights[%u].isValid", currentLightIndex );

        varLocation = openGL.getShaderVariableLocation( uniformName );
        assert( varLocation != -1 );

        if( !( openGL.getShaderInteger( ShaderProgramType::DEFAULT, uniformName ) ) ){
            glUniform1i( varLocation, true );
            return currentLightIndex;
        }
    }

    OpenGL::checkStatus( "Light::lockShaderLight()" );

    // TODO: Or return -1 (change return type to GLint)?
    throw std::runtime_error( "Light::lockShaderLight() - No free light struct in shader" );
}

} // namespace como
