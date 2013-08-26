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

namespace como {

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
    // TODO (here or not): free allocated resources (shader objects, etc).
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
 * 2. Utilities
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


/***
 * 3. Shader loading
 ***/

void ShaderLoader::loadShaderObject( GLenum shaderType, const char* shaderFile )
{
    const GLint STR_SIZE = 1024;
    GLchar* shaderCode = new GLchar[ STR_SIZE ];
    GLchar log[STR_SIZE] = "";
    GLuint shaderObject;
    GLint compilationResult;

    // Create shader object.
    shaderObject = glCreateShader( shaderType );

    // Read shader source from file.
    readFile( shaderFile, shaderCode, STR_SIZE );
    glShaderSource( shaderObject, 1, &shaderCode, &STR_SIZE );

    // Compile shader and check compilation result.
    glCompileShader( shaderObject );
    glGetShaderiv( shaderObject, GL_COMPILE_STATUS, &compilationResult );

    if( compilationResult == GL_FALSE ){
        // TODO: Difference between different shader types.
        glGetShaderInfoLog( shaderObject, STR_SIZE, NULL, log );
        cout << "ERROR compiling shader: " << endl
             << log << endl;
        return;
    }

    // Attach shader object to shader program.
    glAttachShader( shaderProgram, shaderObject );

    delete [] shaderCode;
}


void ShaderLoader::loadMinimumShaderProgram( const char* vertexShaderFile, const char* fragmentShaderFile )
{
    GLint linkingResult;
    const GLint STR_SIZE = 1024;
    GLchar log[STR_SIZE];

    // Load both vertex and fragment shaders.
    loadShaderObject( GL_VERTEX_SHADER, vertexShaderFile );
    loadShaderObject( GL_FRAGMENT_SHADER, fragmentShaderFile );

    // Link the shader program and check the result.
    glLinkProgram( shaderProgram );
    glGetProgramiv( shaderProgram, GL_LINK_STATUS, &linkingResult );

    if( linkingResult == GL_FALSE ){
        glGetProgramInfoLog( shaderProgram, STR_SIZE, NULL, log );

        cout << "ERROR linking shader program" << endl
             << log << endl;

        return;
    }

    // Set the created shader program as the active one.
    glUseProgram( shaderProgram );
    cout << "New shader program loaded and being used!" << endl;
}



} // namespace como
