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
#include <glm/gtc/type_ptr.hpp>

namespace como {

const glm::vec4 DEFAULT_LIGHT_VECTOR = glm::vec4( 0.0f, 1.0f, 0.0f, 0.0f );

/***
 * 1. Construction
 ***/

DirectionalLight::DirectionalLight( const PackableColor& lightColor, const glm::vec3& lightVector, OpenGL& openGL ) :
    Light( LightType::DIRECTIONAL_LIGHT, lightColor, "data/system/primitives/directional_light.prim", openGL ), // TODO: Load material from file.
    directionalLightIndex_( lockShaderDirectionalLight( openGL ) )
{
    (void)( lightVector ); // TODO: Remove this argument.

    GLint currentShaderProgram = 0;
    char uniformName[64];

    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    assert( currentShaderProgram != 0 );

    // Get the location of this light's isValid in the GLSL shader program.
    sprintf( uniformName, "directionalLights[%u].isValid", directionalLightIndex_ );
    isValidLocation_ = glGetUniformLocation( currentShaderProgram, uniformName );
    assert( isValidLocation_ != -1 );

    // Get the location of the DirectionalLight::lightIndex variable in shader.
    sprintf( uniformName, "directionalLights[%u].lightIndex", directionalLightIndex_ );
    lightIndexLocation_ = glGetUniformLocation( currentShaderProgram, uniformName );
    assert( lightIndexLocation_ != -1 );

    // Get the location of the DirectionalLight::lightVector variable in shader.
    sprintf( uniformName, "directionalLights[%u].lightVector", directionalLightIndex_ );
    lightVectorLocation_ = glGetUniformLocation( currentShaderProgram, uniformName );
    assert( lightVectorLocation_ != -1 );

    // Get the location of the DirectionalLight::halfVector variable in shader.
    sprintf( uniformName, "directionalLights[%u].halfVector", directionalLightIndex_ );
    halfVectorLocation_ = glGetUniformLocation( currentShaderProgram, uniformName );

    // TODO: Replace comparison by "!= 1" when halfVectorLocation_ be implemented in shader.
    assert( halfVectorLocation_ == -1 );

    // Set the light index in shader.
    glUniform1i( lightIndexLocation_, getLightIndex() );

    // Initialize light vector in shader.
    update();
}


/***
 * 2. Destruction
 ***/

DirectionalLight::~DirectionalLight()
{
    // Disable this light in shader.
    glUniform1i( isValidLocation_, false );
}


/***
 * 3. Getters
 ***/

glm::vec3 DirectionalLight::getLightVector() const
{
    return lightVector_;
}

glm::vec3 DirectionalLight::getHalfVector() const
{
    return halfVector_;
}


GLint DirectionalLight::getLightIndex() const
{
    return directionalLightIndex_;
}


/***
 * 4. Updating
 ***/

void DirectionalLight::update()
{
    // Update mesh associated to this light.
    Light::update();

    // Compute transformed light vector.
    lightVector_ = glm::normalize( glm::vec3( modelMatrix_ * DEFAULT_LIGHT_VECTOR ) );

    // TODO: Update half vector.
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
 * 6. Shader communication
 ***/

void DirectionalLight::sendToShader( OpenGL &openGL, const glm::mat4& viewMatrix ) const
{
    Light::sendToShader( openGL, viewMatrix );

    glUniform3fv( lightVectorLocation_, 1, glm::value_ptr( glm::mat3( viewMatrix ) * lightVector_ ) );
}


/***
 * 7. Drawing
 ***/

void DirectionalLight::draw( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec4* contourColor ) const
{
    openGL->setShadingMode( ShadingMode::SOLID_PLAIN );

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // Compute MVP matrix and pass it to the shader.
    openGL->setMVPMatrix( modelMatrix_, viewMatrix, projectionMatrix );

    // Bind Mesh VAO and VBOs as the active ones.
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );

    for( const auto& trianglesGroup : trianglesGroups_ ){
        // Send this mesh's material to shader.
        //materials_[polygonsGroup.materialIndex]->sendToShader();

        glDrawElements( GL_TRIANGLES,
                        trianglesGroup.nTriangles * 3,
                        GL_UNSIGNED_INT,
                        ( std::intptr_t* )( trianglesGroup.firstTriangleIndex * 3 * sizeof( GL_UNSIGNED_INT ) ) );
    }


    // Set the color for the mesh's contour.
    // TODO: Use only one condition (force contourColor to be passed as a
    // reference?)
    if( displayEdges_ && ( contourColor != nullptr ) ){
        openGL->setShadingMode( ShadingMode::SOLID_PLAIN );

        glUniform4fv( uniformColorLocation, 1, glm::value_ptr( *contourColor ) );
        OpenGL::checkStatus( "contourColor sent to shader" );

        // Now we'll draw mesh's contour. Set polygon mode for rendering
        // lines.
        // TODO: I still have to use polygon offset.
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        // Draw Mesh's contour
        glDrawElements( GL_TRIANGLES, nEboElements_, GL_UNSIGNED_INT, NULL );

        // Return polygon mode to previos GL_FILL.
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}


/***
 * 9. Auxiliar methods
 ***/

bool DirectionalLight::containsProperty( const void *property ) const
{
    return Mesh::containsProperty( property );
}


// FIXME: Duplicated code (Light::lockShaderLight()).
GLuint DirectionalLight::lockShaderDirectionalLight( OpenGL& openGL )
{
    char uniformName[64] = {0};
    GLint varLocation = -1;
    unsigned int currentLightIndex;

    for( currentLightIndex = 0; currentLightIndex < 4; currentLightIndex++ ){ // Retrieve MAX_DIRECTIONAL_LIGHTS from shader.
        sprintf( uniformName, "directionalLights[%u].isValid", currentLightIndex );

        varLocation = openGL.getShaderVariableLocation( uniformName );
        assert( varLocation != -1 );

        if( !( openGL.getShaderInteger( ShaderProgramType::DEFAULT, uniformName ) ) ){
            glUniform1i( varLocation, true );
            return currentLightIndex;
        }
    }

    OpenGL::checkStatus( "DirectionalLight::lockShaderDirectionalLight()" );

    // TODO: Or return -1 (change return type to GLint)?
    throw std::runtime_error( "DirectionalLight::lockShaderDirectionalLight() - No free light struct in shader" );
}

} // namespace como
