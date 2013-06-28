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

#include "opengl_canvas.hpp"

OpenGLCanvas::OpenGLCanvas(QWidget *parent) :
    QGLWidget(parent)
{
}


void OpenGLCanvas::initializeGL()
{
    //glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );

    glEnable( GL_DEPTH_TEST );

    glDepthFunc( GL_LEQUAL );
}

#include <iostream>
using namespace std;

void OpenGLCanvas::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    cube.draw();
    /*
    glBegin(GL_TRIANGLES);
          glColor3f(1.0f, 0.0f, 0.0f);
          glVertex3f(0.0f,0.0f, 0.0f);
          glVertex3f(2.0f,0.0f, 0.0f);
          glVertex3f(1.0f,1.0f, 0.0f);

          glColor3f(0.0f,1.0f, 0.0f);
          glVertex3f(-1.0f,0.0f, 0.0f);
          glVertex3f(-3.0f,2.0f, 0.0f);
          glVertex3f(-2.0f,0.0f, 0.0f);
    glEnd();
    */

    glFlush();
}
