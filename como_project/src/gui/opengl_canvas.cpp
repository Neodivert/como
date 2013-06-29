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

    ShaderLoader* shaderLoader = ShaderLoader::getInstance();

    shaderLoader->loadShaders( "data/shaders/basicVertexShader.shader", "data/shaders/basicFragmentShader.shader" );

    ShaderLoader::destroy();
}

#include <iostream>
using namespace std;

void OpenGLCanvas::paintGL()
{
    static GLfloat x = 0.0f;
    cout << "Painting" << endl;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


    glPointSize( 10.0f );
    cube.draw();

    glBegin(GL_POINTS);

          //glColor3f(0.0f, 1.0f, 0.0f);

          glVertex3f( 1.0f, 0.0f, 0.0f );

          glVertex3f( 0.0f, 1.0f, 0.0f );


          glVertex3f( 0.0f, -1.0f, 0.0f );

          glVertex3f( -1.0f, 0.0f, 0.0f );
          /*
          glColor3f(0.0f,1.0f, 0.0f);
          glVertex3f(0.0f,0.0f, 0.0f);
          glVertex3f(2.0f,2.0f, 0.0f);
          glVertex3f(1.0f,0.0f, 0.0f);
*/
    glEnd();

    x += 0.5f;

    glFlush();
}


void OpenGLCanvas::resizeGL( int w, int h )
{
    cout << "Resizing" << endl;
    glViewport( 0, 0, w, h );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // http://stackoverflow.com/questions/2571402/c-opengl-glortho-please-explain-the-usage-of-this-command
    glOrtho(0.0f, w, h, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
