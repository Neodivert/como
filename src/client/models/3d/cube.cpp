/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
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
 * 1. Initialization and destruction
 ***/

Cube::Cube( const std::uint8_t* color ) :
    Mesh( color )
{
    // The cube has its center in the origin.
    const GLfloat halfWidth = 0.5f;

    // Cube's default vertices.
    const GLfloat cubeVertices[] =
    {
        // Front face
        halfWidth, -halfWidth, halfWidth,  // V0 : Bottom right
        halfWidth, halfWidth, halfWidth,   // V1 : Top right
        -halfWidth, halfWidth, halfWidth,  // V2 : Top left
        -halfWidth, -halfWidth, halfWidth, // V3 : Bottom left

        // Back face
        halfWidth, -halfWidth, -halfWidth, // V4 : Bottom right
        halfWidth, halfWidth, -halfWidth,  // V5 : Top right
        -halfWidth, halfWidth, -halfWidth, // V6 : Top left
        -halfWidth, -halfWidth, -halfWidth // V7 : Bottom left
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
        4, 5, 0,
        5, 1, 0,

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
}

} // namespace como
