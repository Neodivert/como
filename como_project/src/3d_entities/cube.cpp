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
    /*** TODO: Move to Geometry constructor ***/
    cout << "Geometry constructor" << endl;

    vbos = new GLuint[N_VBOS];

    // Generate a VAO for the geometry.
    glGenVertexArrays( 1, &vao );

    cout << "vao: " << vao << endl;

    // Bind the previous VAO as the active one.
    glBindVertexArray( vao );

    // Generate some VBOs for the geometry's vertices data.
    glGenBuffers( N_VBOS, vbos );

    // Bind one VBO for keeping vertex data.
    glBindBuffer( GL_ARRAY_BUFFER, vbos[VERTEX_DATA] );

    // Bind one VBO for keeping vertex indices.
    //glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbos[VERTEX_INDICES] );
    /******/

    cout << "Cube constructor" << endl;

    const GLfloat cubeVertices[] =
    {
        // Front face
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,

        1.0f, 0.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        0.0f, 1.0f, -1.0f,
        0.0f, 0.0f, -1.0f
    };

    const GLubyte cubeIndices[] =
    {
        0, 1, 3,
        1, 2, 3
    };

    cout << sizeof( cubeVertices ) << endl;
    glBufferData( GL_ARRAY_BUFFER, sizeof( cubeVertices ), cubeVertices, GL_DYNAMIC_DRAW );
    //glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( cubeIndices ), cubeIndices, GL_STATIC_DRAW );

    // Bind one VBO for keeping vertex data.
    glBindBuffer( GL_ARRAY_BUFFER, vbos[VERTEX_DATA] );

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


