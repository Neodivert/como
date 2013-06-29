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

Geometry::Geometry()
{
    // Generate a VAO for the geometry.
    glGenVertexArrays( 1, &vao );

    cout << "vao: " << vao << endl;

    // Bind the previous VAO as the active one.
    glBindVertexArray( vao );

    // Generate some VBOs for the geometry's vertices data.
    glGenBuffers( 1, &vbo );

    // Bind one VBO for keeping vertex data.
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    // Bind one VBO for keeping vertex indices.
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
}


Geometry::~Geometry()
{
    glDeleteBuffers( 1, &vbo );
    //delete [] vbos;
}


void Geometry::draw() const
{
    // Bind Geometry VAO as the active one.
    glBindVertexArray( vao );

    // Draw Geometry primitives.
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL );
}


void Geometry::update()
{
    cout << "Inside update()" << endl;
    cout << "originalVertices.size(): " << originalVertices.size() << endl;

    GLfloat* transformedVertices = NULL;
    glm::vec4 transformedVertex;

    transformedVertices = (GLfloat*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );

    for( GLuint i = 0; i<originalVertices.size(); i++ ){
        transformedVertex = transformationMatrix * glm::vec4( originalVertices[i], 1.0f );

        transformedVertices[i*3+X] = transformedVertex.x;
        transformedVertices[i*3+Y] = transformedVertex.y;
        transformedVertices[i*3+Z] = transformedVertex.z;

        cout << "transformedVertices[" << i << "]: "
             << transformedVertices[i*3+X] << ", "
             << transformedVertices[i*3+Y] << ", "
             << transformedVertices[i*3+Z] << endl;
    }

    glUnmapBuffer( GL_ARRAY_BUFFER );
}
