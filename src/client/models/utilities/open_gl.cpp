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

namespace como {

OpenGL::OpenGL()
{
    // Get the id for the current shader program.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram_ );

    if( currentShaderProgram_ == -1 ){
        throw std::runtime_error( "current shader program == -1");
    }

    // Get location of uniform shader variables.
    shaderVariablesLocations_[ShaderVariable::LIGHTING_ENABLED] =
            getShaderVariableLocation( "lightingEnabled" );
    shaderVariablesLocations_[ShaderVariable::TEXTURING_ENABLED] =
            getShaderVariableLocation( "texturingEnabled" );
}


/***
 * 3. Lighting
 ***/

void OpenGL::enableLighting() const
{
    glUniform1i( shaderVariablesLocations_.at( ShaderVariable::LIGHTING_ENABLED ), 1 );
    OpenGL::checkStatus( "Scene::enableLighting()" );
}


void OpenGL::disableLighting() const
{
    glUniform1i( shaderVariablesLocations_.at( ShaderVariable::LIGHTING_ENABLED ), 0 );
    OpenGL::checkStatus( "Scene::disableLighting()" );
}


/***
 * 4. Texturing
 ***/

void OpenGL::enableTexturing() const
{
    glUniform1i( shaderVariablesLocations_.at( ShaderVariable::TEXTURING_ENABLED ), 1 );
    OpenGL::checkStatus( "Scene::enableTexturing()" );
}


void OpenGL::disableTexturing() const
{
    glUniform1i( shaderVariablesLocations_.at( ShaderVariable::TEXTURING_ENABLED ), 0 );
    OpenGL::checkStatus( "Scene::enableTexturing()" );
}


/***
 * 5. Utilities
 ***/

GLint OpenGL::getShaderVariableLocation( string varName ) const
{
    GLint varLocation = -1;

    varLocation = glGetUniformLocation( currentShaderProgram_, varName.c_str() );

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
        throw std::runtime_error( std::string( "OpenGL ERROR at [" ) +
                                  location +
                                  std::string( "]: " ) +
                                  OpenGL::getOpenGLErrorString( errorCode ) +
                                  std::string( "\n" )
                                  );
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
