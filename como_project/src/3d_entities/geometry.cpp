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
    cout << "Geometry constructor" << endl;

    vbos = new GLuint[N_VBOS];

    // Generate a VAO for the geometry.
    glGenVertexArrays( 1, &vao );

    // Bind the previous VAO as the active one.
    glBindVertexArray( vao );

    // Generate some VBOs for the geometry's vertices data.
    glGenBuffers( N_VBOS, vbos );

    // Bind one VBO for keeping vertex data.
    glBindBuffer( GL_ARRAY_BUFFER, vbos[VERTEX_DATA] );

    // Bind one VBO for keeping vertex indices.
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbos[VERTEX_INDICES] );
}


Geometry::~Geometry()
{
    delete [] vbos;
}


void Geometry::draw() const
{
    glBindVertexArray( vao );

    glBegin(GL_LINE_LOOP);//start drawing a line loop
        //glColor3f( 1.0f, 0.0f, 0.0f );
          glVertex3f(-1.0f,0.0f,0.0f);//left of window
          glVertex3f(0.0f,-1.0f,0.0f);//bottom of window
          glVertex3f(1.0f,0.0f,0.0f);//right of window
          glVertex3f(0.0f,1.0f,0.0f);//top of window
        glEnd();//end drawing of line loop

    glDrawArrays(GL_LINE_LOOP, 0, 3);
    //glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, NULL );
}
