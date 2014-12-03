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

#include "open_gl.hpp"
#include <cassert>
#include <glm/mat3x3.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>             // glm::value_ptr

namespace como {

GLint defaultShaderProgram_;
GLint normalsShaderProgram_;


/***
 * 1. Construction
 ***/

OpenGL::OpenGL() :
    currentProgramID_( 0 )
{
    unsigned int currentLightIndex;
    GLint varLocation = -1;
    char uniformName[64] = {0};

    OpenGL::checkStatus( "OpenGL::OpenGL() - begin" );

    // Load shaders
    msl::ShaderLoader shaderLoader;

    shaderProgramsIDs_[ ShaderProgramType::DEFAULT ] =
            shaderLoader.loadShaderProgram( "data/shaders/basicVertexShader.shader",
                                            "data/shaders/basicFragmentShader.shader" );
    shaderProgramsIDs_[ ShaderProgramType::NORMALS ] =
            shaderLoader.loadShaderProgram( "data/shaders/normals/vertex.shader",
                                            "data/shaders/normals/geometry.shader",
                                            "data/shaders/normals/fragment.shader" );

    // Start using the default shader program.
    setShadingMode( ShadingMode::SOLID_LIGHTING_AND_TEXTURING );

    // Initialize all lights as "invalid".
    for( currentLightIndex = 0; currentLightIndex < 4; currentLightIndex++ ){ // Retrieve MAX_LIGHTS from shader.
        sprintf( uniformName, "lights[%u].isValid", currentLightIndex );
        varLocation = glGetUniformLocation( shaderProgramsIDs_[ ShaderProgramType::DEFAULT ], uniformName );
        glUniform1i( varLocation, false );
    }

    // Initialize all directional lights as "invalid".
    for( currentLightIndex = 0; currentLightIndex < 4; currentLightIndex++ ){ // Retrieve MAX_LIGHTS from shader.
        sprintf( uniformName, "directionalLights[%u].isValid", currentLightIndex );
        varLocation = glGetUniformLocation( shaderProgramsIDs_[ ShaderProgramType::DEFAULT ], uniformName );
        glUniform1i( varLocation, false );
    }

    OpenGL::checkStatus( "OpenGL::OpenGL() - end" );
}


/***
 * 2. Destruction
 ***/

OpenGL::~OpenGL()
{
    // Free shader programs.
    for( const std::pair< ShaderProgramType, GLuint >& shaderProgramPair : shaderProgramsIDs_ ){
        glDeleteProgram( shaderProgramPair.second );
    }
}


/***
 * 3. Getters
 ***/

ShadingMode OpenGL::getShadingMode() const
{
    LOCK
    return currentShadingMode_;
}


GLint OpenGL::getShaderInteger( ShaderProgramType shaderProgramType, std::string varName )
{
    LOCK

    GLint varLocation = -1;
    GLint varValue = 0;

    varLocation = getShaderVariableLocation( varName,
                               shaderProgramsIDs_.at( shaderProgramType ) );

    glGetUniformiv( shaderProgramsIDs_.at( shaderProgramType ), varLocation, &varValue );

    return varValue;
}


GLuint OpenGL::getShaderProgramID( ShaderProgramType shaderProgramType ) const
{
    LOCK

    return shaderProgramsIDs_.at( shaderProgramType );
}


/***
 * 4. Setters
 ***/

void OpenGL::setShadingMode( ShadingMode shadingMode )
{
    LOCK

    currentShadingMode_ = shadingMode;

    switch( shadingMode ){
        case ShadingMode::SOLID_LIGHTING_AND_TEXTURING:
            setProgram( ShaderProgramType::DEFAULT );
            enableLighting();
            enableTexturing();
        break;
        case ShadingMode::SOLID_LIGHTING:
            setProgram( ShaderProgramType::DEFAULT );
            enableLighting();
            disableTexturing();
        break;
        case ShadingMode::SOLID_PLAIN:
            setProgram( ShaderProgramType::DEFAULT );
            disableLighting();
            disableTexturing();
        break;
        case ShadingMode::NORMALS:
            setProgram( ShaderProgramType::NORMALS );
        break;
    }
}


void OpenGL::setMVPMatrix( const glm::mat4& modelMatrix , const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix )
{
    LOCK

    GLint uniformLocation = -1;
    const glm::mat4& modelViewMatrix = viewMatrix * modelMatrix;
    const glm::mat4& mvpMatrix = projectionMatrix * modelViewMatrix;

    // Send the given MVP matrix to shader.
    uniformLocation = glGetUniformLocation( currentProgramID_, "mvpMatrix" );
    assert( uniformLocation != -1 );
    glUniformMatrix4fv( uniformLocation, 1, GL_FALSE, &mvpMatrix[0][0] );

    // Compute normal matrix and send it to shader.
    glm::mat3 normalMatrix = glm::mat3( glm::transpose( glm::inverse( modelViewMatrix ) ) );
    if( currentProgramID_ == shaderProgramsIDs_.at( ShaderProgramType::NORMALS ) ){
        //normalMatrix = glm::mat3( projectionMatrix * glm::mat4( normalMatrix ) );
        normalMatrix = glm::mat3( projectionMatrix ) * normalMatrix;
    }

    uniformLocation = glGetUniformLocation( currentProgramID_, "normalMatrix" );
    assert( uniformLocation != -1 );
    glUniformMatrix3fv( uniformLocation, 1, GL_FALSE, &normalMatrix[0][0] );

    checkStatus( "OpenGL::setMVPMatrix()" );
}


void OpenGL::setUniformInteger( GLint location, GLint value )
{
    glUniform1i( location, value );
}


void OpenGL::setUniformVec3( GLint location, const glm::vec3& value )
{
    glUniform3fv( location, 1, glm::value_ptr( value ) );
}


void OpenGL::setUniformVec4( GLint location, const glm::vec4& value )
{
    glUniform4fv( location, 1, glm::value_ptr( value ) );
}


/***
 * 5. Utilities
 ***/

GLint OpenGL::getShaderVariableLocation( std::string varName, GLint program ) const
{
    LOCK

    if( program < 0 ){
        program = shaderProgramsIDs_.at( ShaderProgramType::DEFAULT );
    }

    GLint varLocation = -1;

    assert( program != -1 );

    varLocation = glGetUniformLocation( program, varName.c_str() );

    if( varLocation < 0 ){
        throw std::runtime_error( std::string( "Shader variable [" ) +
                                  varName +
                                  "] not found" );
    }

    return varLocation;
}


/***
 * 6. Checking
 ***/

void OpenGL::checkStatus( std::string location )
{
    GLenum errorCode = glGetError();

    if( errorCode != GL_NO_ERROR ){
        std::string errorMessage =
                std::string( "OpenGL ERROR at [" ) +
                location +
                std::string( "]: " ) +
                OpenGL::getOpenGLErrorString( errorCode ) +
                std::string( "\n" );
        throw std::runtime_error( errorMessage );
    }
}

std::string OpenGL::getOpenGLErrorString( GLenum errorCode )
{
    switch( errorCode ){
        case GL_NO_ERROR:
            return std::string( "No error" );
        break;
        case GL_INVALID_ENUM:
            return std::string( "Invalid enum" );
        break;
        case GL_INVALID_VALUE:
            return std::string( "Invalid value" );
        break;
        case GL_INVALID_OPERATION:
            return std::string( "Invalid operation" );
        break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return std::string( "Invalid framebuffer operation" );
        break;
        case GL_OUT_OF_MEMORY:
            return std::string( "Out of memory" );
        break;
        case GL_STACK_UNDERFLOW:
            return std::string( "Stack underflow" );
        break;
        case GL_STACK_OVERFLOW:
            return std::string( "Stack overflow" );
        break;
        default:
            return std::string( "Unknown error" );
        break;
    }
}


/***
 * 8. Setters (private)
 ***/

void OpenGL::setProgram( ShaderProgramType program )
{
    if( currentProgramID_ != shaderProgramsIDs_.at( program) ){
        currentProgramID_ = shaderProgramsIDs_.at( program );
        glUseProgram( currentProgramID_ );
    }
}


/***
 * 9. Lighting
 ***/

void OpenGL::enableLighting() const
{
    GLint uniformLocation = getShaderVariableLocation( "lightingEnabled" );
    glUniform1i( uniformLocation, 1 );
}


void OpenGL::disableLighting() const
{
    GLint uniformLocation = getShaderVariableLocation( "lightingEnabled" );
    glUniform1i( uniformLocation, 0 );
}


/***
 * 10. Texturing
 ***/

void OpenGL::enableTexturing() const
{
    GLint uniformLocation = getShaderVariableLocation( "texturingEnabled" );
    glUniform1i( uniformLocation, 1 );
}


void OpenGL::disableTexturing() const
{
    GLint uniformLocation = getShaderVariableLocation( "texturingEnabled" );
    glUniform1i( uniformLocation, 0 );
}


} // namespace como
