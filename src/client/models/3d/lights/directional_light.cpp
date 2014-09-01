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

DirectionalLight::DirectionalLight( GLuint directionalLightIndex, GLint lightIndex, const PackableColor& lightColor, const glm::vec3& lightVector, MaterialConstPtr meshMaterial ) :
    Light( LightType::DIRECTIONAL_LIGHT, lightIndex, lightColor, "data/system/primitives/directional_light.prim", meshMaterial ) // TODO: Load material from file.
{
    (void)( lightVector ); // TODO: Remove this argument.

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
    setLightVector( glm::vec3( DEFAULT_LIGHT_VECTOR ) );
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
    setLightVector( glm::normalize( glm::vec3( modelMatrix_ * DEFAULT_LIGHT_VECTOR ) ) );
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
 * 6. Drawing
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

    for( auto polygonsGroup : polygonsGroups_ ){
        // Send this mesh's material to shader.
        materials_[polygonsGroup.materialIndex]->sendToShader();

        glDrawElements( GL_TRIANGLES,
                        polygonsGroup.nTriangles * 3,
                        GL_UNSIGNED_INT,
                        ( std::intptr_t* )( polygonsGroup.firstTriangleIndex * 3 * sizeof( GL_UNSIGNED_INT ) ) );
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
 * 8. Auxiliar methods
 ***/

bool DirectionalLight::containsProperty( const void *property ) const
{
    return Mesh::containsProperty( property );
}

} // namespace como
