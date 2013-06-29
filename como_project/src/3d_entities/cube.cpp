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

    const GLubyte cubeIndices[] =
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

    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( cubeIndices ), cubeIndices, GL_STATIC_DRAW );

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


