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

namespace como {

GLint defaultShaderProgram_;
GLint normalsShaderProgram_;

OpenGL::OpenGL() :
    currentProgramID_( -1 )
{
    OpenGL::checkStatus( "OpenGL::OpenGL() - begin" );

    // Load shaders
    msl::ShaderLoader* shaderLoader = msl::ShaderLoader::getInstance();

    shaderProgramsIDs_[ ShaderProgramType::DEFAULT ] =
            shaderLoader->loadShaderProgram( "data/shaders/basicVertexShader.shader",
                                             "data/shaders/basicFragmentShader.shader" );
    assert( shaderProgramsIDs_.at( ShaderProgramType::DEFAULT ) != -1 );

    shaderProgramsIDs_[ ShaderProgramType::NORMALS ] =
            shaderLoader->loadShaderProgram( "data/shaders/normals/vertex.shader",
                                             "data/shaders/normals/geometry.shader",
                                             "data/shaders/normals/fragment.shader" );
    assert( shaderProgramsIDs_.at( ShaderProgramType::NORMALS ) != -1 );

    shaderLoader->destroy();

    // Start using the default shader program.
    setShadingMode( ShadingMode::SOLID_LIGHTING_AND_TEXTURING );

    OpenGL::checkStatus( "OpenGL::OpenGL() - end" );
}


/***
 * 3. Setters
 ***/

void OpenGL::setShadingMode( ShadingMode shadingMode )
{
    switch( shadingMode ){
        case ShadingMode::SOLID_LIGHTING_AND_TEXTURING:
            setProgram( ShaderProgramType::DEFAULT );
            enableLighting();
            enableTexturing();
        break;
        case ShadingMode::SOLID_LIGHTING:
            setProgram( ShaderProgramType::DEFAULT );
            enableLighting();
            enableTexturing();
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
    GLint uniformLocation = -1;
    const glm::mat4& modelViewMatrix = viewMatrix * modelMatrix;
    const glm::mat4& mvpMatrix = projectionMatrix * modelViewMatrix;

    // Send the given MVP matrix to shader.
    uniformLocation = glGetUniformLocation( currentProgramID_, "mvpMatrix" );
    assert( uniformLocation != -1 );
    glUniformMatrix4fv( uniformLocation, 1, GL_FALSE, &mvpMatrix[0][0] );

    // Compute normal matrix and send it to shader.
    const glm::mat3 normalMatrix = glm::mat3( glm::transpose( glm::inverse( modelViewMatrix ) ) );

    uniformLocation = glGetUniformLocation( currentProgramID_, "normalMatrix" );
    assert( uniformLocation != -1 );
    glUniformMatrix3fv( uniformLocation, 1, GL_FALSE, &normalMatrix[0][0] );

    checkStatus( "OpenGL::setMVPMatrix()" );
}


void OpenGL::setProgram( ShaderProgramType program )
{
    if( currentProgramID_ != shaderProgramsIDs_.at( program) ){
        currentProgramID_ = shaderProgramsIDs_.at( program );
        glUseProgram( currentProgramID_ );
    }
}


/***
 * 4. Lighting
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
 * 5. Texturing
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


/***
 * 6. Utilities
 ***/

GLint OpenGL::getShaderVariableLocation( string varName ) const
{
    GLint varLocation = -1;

    assert( currentProgramID_ != -1 );

    varLocation = glGetUniformLocation( currentProgramID_, varName.c_str() );

    if( varLocation < 0 ){
        throw std::runtime_error( std::string( "Shader variable [" ) +
                                  varName +
                                  "] not found" );
    }

    return varLocation;
}


/***
 * 7. Checking
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

} // namespace como
