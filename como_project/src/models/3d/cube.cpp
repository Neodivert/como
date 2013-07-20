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


namespace como {

/***
 * 1. Initialization
 ***/

Cube::Cube()
{
    // Cube's default vertices.
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

    // Vertex indices for the cube's faces.
    const GLubyte cubeElements[] =
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

    // Copy vertices to mesh.
    setVertices( 8, cubeVertices );

    // Copy elements (indices) to mesh.
    setElements( sizeof( cubeElements ) / sizeof( cubeElements[0] ), cubeElements );

    // Update transformed vertices (original vertices * transformation matrix).
    //update();

    // Get the location of the input variable "vPosition" for the current shader program.
    GLint prog;
    glGetIntegerv( GL_CURRENT_PROGRAM, &prog );
    GLint vPosition = glGetAttribLocation( prog, "vPosition" );

    if( vPosition == GL_INVALID_OPERATION ){
        cout << "Error getting layout of \"position\"" << endl;
    }else{
        cout << "vPosition: (" << vPosition << ")" << endl;
    }

    // By using the previous "vPosition" position, specify the location and data format of
    // the array of vertex positions.
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );

    // Enable previous array of vertex positions.
    glEnableVertexAttribArray( vPosition );
}

} // namespace como
