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
#include <iostream>
using namespace std;

/***
 * 1. Initialization and destruction
 ***/

OpenGLCanvas::OpenGLCanvas(QWidget *parent) :
    QGLWidget(parent)
{
    cube = NULL;
}


OpenGLCanvas::~OpenGLCanvas()
{
    delete cube;
}


void OpenGLCanvas::initializeGL()
{
    // Set clear color.
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    // Load shaders.
    ShaderLoader* shaderLoader = ShaderLoader::getInstance();
    shaderLoader->loadMinimumShaderProgram( "data/shaders/basicVertexShader.shader", "data/shaders/basicFragmentShader.shader" );
    ShaderLoader::destroy();

    // Set OpenGL depth test.
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    // Initialize geometry.
    cube = new Cube;
}


/***
 * 2. Updating and drawing
 ***/

void OpenGLCanvas::paintGL()
{
    cout << "Drawing GL" << endl;

    // Clear buffers.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Draw geometries.
    cube->draw();

    // Flush.
    glFlush();
}


void OpenGLCanvas::resizeGL( int w, int h )
{
    cout << "Resizing" << endl;

    // Viewport occuppies the full canvas.
    glViewport( 0, 0, w, h );

    // Set a orthographic projection cube.
    // http://stackoverflow.com/questions/2571402/c-opengl-glortho-please-explain-the-usage-of-this-command
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, w, h, 0.0f, 0.0f, 1.0f);

    // Initialize modelview matrix to identity one.
    // TODO: is this necessary?
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
