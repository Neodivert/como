/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "shader_loader.hpp"
#include <iostream>
using namespace std;

ShaderLoader* ShaderLoader::instance = NULL;


/***
 * 1. Initialization and destruction
 ***/

ShaderLoader::ShaderLoader()
{
    shaderProgram = glCreateProgram();
    if( shaderProgram == 0 ){
        cout << "ERROR in glCreateProgram()" << endl;
        return;
    }
}

ShaderLoader::~ShaderLoader()
{
    //delete [] shaders;
}


ShaderLoader* ShaderLoader::getInstance()
{
    if( instance == NULL ){
        instance = new ShaderLoader();
    }
    return instance;
}


void ShaderLoader::destroy()
{
    delete instance;
    instance = NULL;
}


/***
 * 2.
 ***/
void ShaderLoader::readFile( const char* filePath, GLchar* buffer, const unsigned int n )
{
    // Open given file.
    std::ifstream file( filePath );
    if( !file.is_open() ){
        return;
    }

    // Copy file content to buffer and append a null character.
    file.read( buffer, n-1 );
    buffer[file.gcount()] = 0;

    // Close file.
    file.close();
}


void ShaderLoader::loadShader( GLenum shaderType, const char* shaderFile )
{
    const GLint STR_SIZE = 1024;
    GLchar* shaderCode = new GLchar[ STR_SIZE ];
    GLuint shaderObject;
    GLint compilationResult;

    // Create shader object.
    shaderObject = glCreateShader( shaderType );

    // Read shader source from file.
    readFile( shaderFile, shaderCode, STR_SIZE );
    cout << shaderCode << endl;
    glShaderSource( shaderObject, 1, &shaderCode, &STR_SIZE );

    // Compile shader and check compilation result.
    glCompileShader( shaderObject );
    glGetShaderiv( shaderObject, GL_COMPILE_STATUS, &compilationResult );

    if( compilationResult == GL_FALSE ){
        // TODO: Difference between different shader types.
        cout << "ERROR compiling shader" << endl;
        return;
    }

    // Attach shader object to shader program.
    glAttachShader( shaderProgram, shaderObject );

    delete [] shaderCode;
}

void ShaderLoader::loadShaders( const char* vertexShaderFile, const char* fragmentShaderFile )
{
    GLint linkingResult;

    loadShader( GL_VERTEX_SHADER, vertexShaderFile );
    loadShader( GL_FRAGMENT_SHADER, fragmentShaderFile );


    glLinkProgram( shaderProgram );
    glGetProgramiv( shaderProgram, GL_LINK_STATUS, &linkingResult );

    if( linkingResult == GL_FALSE ){
        cout << "ERROR linking shader program" << endl;
        return;
    }


    glUseProgram( shaderProgram );
    cout << "New shader program loaded and being used!" << endl;
}

