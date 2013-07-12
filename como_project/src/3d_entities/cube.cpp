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

    // Vertex indices for defining the cube's contour.
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


    // Compute the number of vertex indices for the cube's both faces and contour.
    nInnerElements = sizeof( cubeInnerElements ) / sizeof( cubeInnerElements[0] );
    nContourElements = sizeof( cubeContourElements ) / sizeof( cubeContourElements[0] );

    // Copy default cube's vertices to this geometry's original vertices.
    originalVertices.resize( N_CUBE_VERTICES );
    for( GLuint i=0; i<N_CUBE_VERTICES; i++ )
    {
        originalVertices[i] = glm::vec3( cubeVertices[i*3+X],
                                         cubeVertices[i*3+Y],
                                         cubeVertices[i*3+Z] );
    }

    // Copy the geometry's elements to a VBO.
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( cubeInnerElements ) + sizeof( cubeContourElements ), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, sizeof( cubeInnerElements ), cubeInnerElements );
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, sizeof( cubeInnerElements ), sizeof( cubeContourElements ), cubeContourElements );

    // Copy original triangles to this geometry's triangles.
    triangles.resize( nInnerElements / 3 );
    cout << "nInnerElements / 3: " << (nInnerElements / 3) << endl;
    for( GLint i = 0; i<nInnerElements/3; i++ ){
        triangles[i][0] = cubeInnerElements[i*3];
        triangles[i][1] = cubeInnerElements[i*3+1];
        triangles[i][2] = cubeInnerElements[i*3+2];
    }

    // Allocate a VBO for cube's transformed vertices.
    glBufferData( GL_ARRAY_BUFFER, sizeof( cubeVertices ), NULL, GL_DYNAMIC_DRAW );

    // Update transformed vertices (original vertices * transformation matrix).
    update();

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


