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
/*
OpenGLCanvas::OpenGLCanvas(QWidget *parent) //:
    //QWindow(parent)
{
    scene = NULL;
    setFocusPolicy( Qt::StrongFocus );

    setAcceptDrops( true );
}
*/

OpenGLCanvas::OpenGLCanvas( shared_ptr<QOpenGLContext> oglContext, shared_ptr<Scene> scene, QWidget *parent ) //:
    //QWindow( parent )
{
    // We will render using OpenGL.
    setSurfaceType( QWindow::OpenGLSurface );

    //create();

    //setWindowFlags(Qt::Widget);

    //context->makeCurrent( this );

    //cout << "OpenGLCanvas created. QGLContext is valid?: " << this->context()->isValid() << ")" << endl;

    /*
    context = create_context();
     setFormat( context->format() );
     context->setDevice( this );
     setContext( context );
    */
    /*
    context->setDevice( this );
    setContext( context );
*/

    cout << "OpenGLCanvas constructor" << endl;

    this->scene = scene;
    this->oglContext = oglContext;

    //setFocusPolicy( Qt::StrongFocus );

    //setAcceptDrops( true );

    //cout << "\tmakeCurrent: " << oglContext->makeCurrent( this ) << endl;
    //initializeOpenGLFunctions();


}


OpenGLCanvas::~OpenGLCanvas()
{
    //delete scene;
}

void OpenGLCanvas::render(QPainter *painter)
{
    Q_UNUSED(painter);
}



void OpenGLCanvas::initialize()
{
    cout << "OpenGLCanvas::initialize()" << endl;
    cout << "\tmakeCurrent: " << oglContext->makeCurrent( this ) << endl;

    // Viewport occuppies the full canvas.
    glViewport( 0, 0, width(), height() );
}



bool OpenGLCanvas::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        render();
        return true;
    default:
        return QWindow::event(event);
    }
}

void OpenGLCanvas::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        render();
}

void OpenGLCanvas::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        render();
}

/*
void OpenGLCanvas::initializeGL()
{
    cout << "OpenGLCanvas::initializeGL()" << endl;
    // Set clear color.
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

    // Set OpenGL depth test.
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
}
*/

/***
 * 2. Events
 ***/
/*
void OpenGLCanvas::keyPressEvent( QKeyEvent *e )
{
    cout << "Key press event" << endl;
  switch ( e->key() )
  {
    case Qt::Key_Left:
      scene->translateSelectedDrawables( -0.01f, 0.0f, 0.0f );
    break;
    case Qt::Key_Right:
      scene->translateSelectedDrawables( 0.01f, 0.0f, 0.0f );
    break;
    default:
      cout << "Unknown key (" << e->key() << ")" << endl;
    break;
  }
  //updateGL();
}


void OpenGLCanvas::dragMoveEvent(QDragMoveEvent *dragMoveEvent )
{
    // The event needs to be accepted here
    cout << "Drag move event accepted" << endl;
    dragMoveEvent->accept();
}

void OpenGLCanvas::mouseMoveEvent( QMouseEvent *mouseMoveEvent )
{
    mouseMoveEvent->accept();
    cout << "Mouse move event (" << mouseMoveEvent->x() << ", " << mouseMoveEvent->y() << ")!" << endl;
}
*/
/***
 * 3. Updating and drawing
 ***/
/*
void OpenGLCanvas::paintGL()
{
    cout << "Drawing GL" << endl;
    glViewport( 0, 0, width(), height() );

    // Clear buffers.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Draw geometries.
    //glViewport( 0, 0, width()/2, height()/2 );
    //scene->draw();

    /*glViewport( width()/2, height()/2, width()/2, height()/2 );
    scene->draw();
    */
    // Flush.
    /*glFlush();
}*/

/*

void OpenGLCanvas::resizeGL( int w, int h )
{
    cout << "Resizing" << endl;

    // Viewport occuppies the full canvas.
    glViewport( 0, 0, w, h );
    /*
    // Set a orthographic projection cube.
    // http://stackoverflow.com/questions/2571402/c-opengl-glortho-please-explain-the-usage-of-this-command
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, w, h, 0.0f, 0.0f, 1.0f);

    // Initialize modelview matrix to identity one.
    // TODO: is this necessary?
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}*/


void OpenGLCanvas::render()
{
    initialize();
    initializeOpenGLFunctions();

    glViewport( 0, 0, width(), height() );

    cout << "OpenGLCanvas::render()" << endl;

    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

    cout << "\tisValid: " << oglContext->isValid() << endl;
    cout << "\tmakeCurrent: " << oglContext->makeCurrent( this ) << endl;
    glClear( GL_COLOR_BUFFER_BIT );

    // TODO: move
    /*
    cout << "renderOld" << endl;
    if (!m_device)
        m_device = new QOpenGLPaintDevice;

    m_device->setSize(size());

    QPainter painter(m_device);
    render(&painter);
    */

    glFlush();
}
