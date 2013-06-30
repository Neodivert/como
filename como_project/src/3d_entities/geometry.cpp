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

#include "geometry.hpp"

#include <X11/X.h>
#include <GL/gl.h>
#include <GL/glx.h>


/***
 * 1. Initialization and destruction
 ***/

Geometry::Geometry()
{   
    // Generate a VAO for the geometry and bind it as the active one.
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Generate a VBO and bind it for holding vertex data.
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    // Generate an EBO and bind it for holding vertex indices.
    glGenBuffers( 1, &ebo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
}


Geometry::~Geometry()
{
    // Tell OpenGL we are done with allocated buffer objects and
    // vertex attribute arrays.
    glDeleteBuffers( 1, &vbo );
    glDeleteBuffers( 1, &ebo );
    glDeleteVertexArrays( 1, &vao );
}


/***
 * 2. Update and drawing.
 ***/

void Geometry::update()
{
    GLfloat* transformedVertices = NULL;
    glm::vec4 transformedVertex;

    // Map the OpenGL's VBO for transformed vertices to client memory, so we can update it.
    transformedVertices = (GLfloat*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );

    // Recompute each transformed vertex by multiplying its corresponding original vertex
    // by transformation matrix.
    for( GLuint i = 0; i<originalVertices.size(); i++ ){
        transformedVertex = transformationMatrix * glm::vec4( originalVertices[i], 1.0f );

        transformedVertices[i*3+X] = transformedVertex.x;
        transformedVertices[i*3+Y] = transformedVertex.y;
        transformedVertices[i*3+Z] = transformedVertex.z;
    }

    // We finished updating the VBO, unmap it so OpenGL can take control over it.
    glUnmapBuffer( GL_ARRAY_BUFFER );
}


void Geometry::draw() const
{
    // Bind Geometry VAO as the active one.
    glBindVertexArray( vao );

    // Draw Geometry primitives.
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL );
}



