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
    cout << "OpenGLCanvas constructor ..." << endl;

    this->scene = scene;
    this->oglContext = oglContext;

    // We will render using OpenGL.
    setSurfaceType( QWindow::OpenGLSurface );

    setFormat( oglContext->format() );
    create();
    cout << "\tOpenGLCanvas created. OpenGL context is valid?: " << oglContext->isValid() << ")" << endl;

    // context->setDevice( this );

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

    initializeGL();

    cout << "OpenGLCanvas constructor ...OK" << endl;



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


void OpenGLCanvas::initializeGL()
{
    cout << "OpenGLCanvas::initializeGL()" << endl;
    cout << "\tmakeCurrent: " << oglContext->makeCurrent( this ) << endl;

    // Set clear color.

}


/***
 * 2. Events
 ***/

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

/*
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


void OpenGLCanvas::render()
{
    cout << "OpenGLCanvas::render() ..." << endl;

    cout << "\tisValid: " << oglContext->isValid() << endl;
    cout << "\tmakeCurrent: " << oglContext->makeCurrent( this ) << endl;

    //oglContext->versionFunctions()->initializeOpenGLFunctions();

    // Viewport occuppies the full canvas.
    glViewport( 0, 0, width(), height() );


    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


    scene->draw();
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

    oglContext->swapBuffers( this );
    cout << "OpenGLCanvas::render() ...OK" << endl;
}
