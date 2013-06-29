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

ShaderLoader::ShaderLoader()
{
}


void ShaderLoader::readFile( const char* filePath, GLchar* buffer, const unsigned int n )
{
    std::ifstream file( filePath );

    if( !file.is_open() ){
        return;
    }

    file.read( buffer, n-1 );

    buffer[file.gcount()] = 0;

    file.close();
}

#include <iostream>
using namespace std;

void ShaderLoader::loadShaders( const char* vertexShaderFile, const char* fragmentShaderFile )
{
    const GLint STR_SIZE = 1024;

    GLchar* vertexShaderCode = new GLchar [ STR_SIZE ];
    GLchar* fragmentShaderCode = new GLchar [ STR_SIZE ];

    readFile( vertexShaderFile, vertexShaderCode, STR_SIZE );
    cout << vertexShaderCode << endl;

    readFile( fragmentShaderFile, fragmentShaderCode, STR_SIZE );
    cout << fragmentShaderCode << endl;


    GLuint shaderProgram, vertexShader, fragmentShader;

    shaderProgram = glCreateProgram();
    if( shaderProgram == 0 ){
        cout << "ERROR in glCreateProgram()" << endl;
        return;
    }

    vertexShader = glCreateShader( GL_VERTEX_SHADER );
    if( vertexShader == 0 ){
        cout << "ERROR in glCreateProgram()" << endl;
        return;
    }

    GLint compilationResult, linkingResult;
    glShaderSource( vertexShader, 1, &vertexShaderCode, &STR_SIZE );
    glCompileShader( vertexShader );
    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &compilationResult );

    if( compilationResult == GL_FALSE ){
        cout << "ERROR compiling vertex shader" << endl;
        return;
    }

    glAttachShader( shaderProgram, vertexShader );

    fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    if( fragmentShader == 0 ){
        cout << "ERROR in glCreateProgram()" << endl;
        return;
    }
    glShaderSource( fragmentShader, 1, &fragmentShaderCode, &STR_SIZE );

    glCompileShader( fragmentShader );
    glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &compilationResult );

    if( compilationResult == GL_FALSE ){
        cout << "ERROR compiling fragment shader" << endl;
        return;
    }

    glAttachShader( shaderProgram, fragmentShader );


    glLinkProgram( shaderProgram );
    /*glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &compilationResult );

    if( compilationResult == GL_FALSE ){
        cout << "ERROR compiling fragment shader" << endl;
        return;
    }*/


    glUseProgram( shaderProgram );


    delete [] vertexShaderCode;
    delete [] fragmentShaderCode;

    cout << "New shader program loaded!" << endl;
}

