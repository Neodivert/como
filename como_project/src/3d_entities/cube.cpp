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

#include "cube.hpp"

Cube::Cube()
{
    const GLfloat cubeVertices[] =
    {
        // Front face
        0.5f, -0.5f, 0.5f,  // V1 : Bottom right
        0.5f, 0.5f, 0.5f,   // V2 : Top right
        -0.5f, 0.5f, 0.5f,  // V3 : Top left
        -0.5f, -0.5f, 0.5f, // V4 : Bottom left

        // Back face
        0.5f, -0.5f, -0.5f, // V5 : Bottom right
        0.5f, 0.5f, -0.5f,  // V6 : Top right
        -0.5f, 0.5f, -0.5f, // V7 : Top left
        -0.5f, -0.5f, -0.5f // V8 : Bottom left
    };

    const GLubyte cubeInnerElements[] =
    {
        // Front face
        0, 1, 3,
        1, 2, 3,

        // Left face
        3, 2, 7,
        2, 6, 7,

        // Back face
        7, 6, 4,
        6, 5, 4,

        // Right face
        4, 5, 7,
        5, 1, 7,

        // Top face
        1, 5, 2,
        5, 6, 2,

        // Bottom face
        4, 0, 7,
        0, 3, 7
    };

    const GLubyte cubeContourElements[] =
    {
        // Front face
        0, 1,
        1, 2,
        2, 3,
        3, 0,

        // Back face
        4, 5,
        5, 6,
        6, 7,
        7, 4,

        // Joins
        0, 4,
        1, 5,
        2, 6,
        3, 7
    };


    nInnerElements = sizeof( cubeInnerElements ) / sizeof( cubeInnerElements[0] );
    nContourElements = sizeof( cubeContourElements ) / sizeof( cubeContourElements[0] );

    cout << "nInnerElements: " << nInnerElements << "(36)" << endl;
    cout << "nContourElements: " << nContourElements << "(24)" << endl;

    originalVertices.resize( N_CUBE_VERTICES );
    for( GLuint i=0; i<N_CUBE_VERTICES; i++ )
    {
        originalVertices[i] = glm::vec3( cubeVertices[i*3+X],
                                         cubeVertices[i*3+Y],
                                         cubeVertices[i*3+Z] );

        cout << "originalVertices[" << i << "]: "
             << originalVertices[i].x << ", "
             << originalVertices[i].y << ", "
             << originalVertices[i].z << ")" << endl;
    }

    cout << sizeof( cubeVertices ) << endl;

    cout << "glBufferData ..." << endl;

    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( cubeInnerElements ) + sizeof( cubeContourElements ), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, sizeof( cubeInnerElements ), cubeInnerElements );
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, sizeof( cubeInnerElements ), sizeof( cubeContourElements ), cubeContourElements );

    cout << "glBufferData ...OK" << endl;

    cout << "Mapping buffer ..." << endl;

    // Map the OpenGL's VBO for transformed vertices to client memory, so we can update it.
    GLubyte* elements = (GLubyte*)glMapBuffer( GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY );

    if( glGetError() != GL_NO_ERROR ){
        cout << "ERROR" << endl;
    }

    cout << "elements: [" << elements << "]" << endl;

    // Recompute each transformed vertex by multiplying its corresponding original vertex
    // by transformation matrix.
    for( GLuint i = 0; i<24+36; i++ ){
        cout << "elements[" << i << "] : " << (GLint)elements[i] << endl;
    }

    // We finished updating the VBO, unmap it so OpenGL can take control over it.
    glUnmapBuffer( GL_ELEMENT_ARRAY_BUFFER );

    cout << "Mapping buffer ...OK" << endl;

    glBufferData( GL_ARRAY_BUFFER, sizeof( cubeVertices ), NULL, GL_DYNAMIC_DRAW );

    cout << "calling to update()" << endl;
    update();

    // Bind one VBO for keeping vertex data.
    //glBindBuffer( GL_ARRAY_BUFFER, vbo );

    GLint prog;
    glGetIntegerv( GL_CURRENT_PROGRAM, &prog );
    GLint vPosition = glGetAttribLocation( prog, "vPosition" );

    if( vPosition == GL_INVALID_OPERATION ){
        cout << "Error getting layout of \"position\"" << endl;
    }else{
        cout << "vPosition: (" << vPosition << ")" << endl;
    }

    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( vPosition );
}


