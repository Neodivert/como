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

OpenGLCanvas::OpenGLCanvas( shared_ptr< ComoApp > comoApp ) :
    QWindow()
{
    // Make this canvas share the given app's state.
    this->comoApp = comoApp;

    // We will render using OpenGL.
    setSurfaceType( QWindow::OpenGLSurface );

    // Set this surface to the same format used by the app's shared OpenGL context.
    setFormat( comoApp->getOpenGLContext()->format() );
    create();
}


/***
 * 2. Events
 ***/

bool OpenGLCanvas::event(QEvent *event)
{
    switch (event->type()) {
        case QEvent::UpdateRequest:
            // Update requested. Render the surface.
            render();

            // Returning true prevents the event from being sent on to other objects.
            return true;
        break;
        default:
            return QWindow::event(event);
        break;
    }
}


void OpenGLCanvas::exposeEvent( QExposeEvent *event )
{
    // We are not interested in using the argument "event". The following macro prevents the
    // compiler from giving us a warning.
    Q_UNUSED(event);

    // Render this OpenGL Canvas if it's exposed.
    if ( isExposed() ){
        render();
    }
}

void OpenGLCanvas::resizeEvent(QResizeEvent *event)
{
    // We are not interested in using the argument "event". The following macro prevents the
    // compiler from giving us a warning.
    Q_UNUSED(event);

    // Render this OpenGL Canvas if it's exposed.
    if( isExposed() ){
        render();
    }
}


void OpenGLCanvas::mousePressEvent( QMouseEvent* mousePressEvent )
{
    // http://en.wikibooks.org/wiki/OpenGL_Programming/Object_selection
    glm::vec4 viewport = glm::vec4( 0, 0, width(), height() );
    glm::vec3 wincoord = glm::vec3( mousePressEvent->x(), height() - mousePressEvent->y() - 1, 0.0f );
    glm::mat4 projection = glm::ortho( -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f );

    glm::vec3 objcoord = glm::unProject( wincoord, camera.getTransformationMatrix(), projection, viewport );

    cout << "y: " << (height() - mousePressEvent->y() - 1) << endl;
    //
    cout << "Coordinates in object space (zNear): ("
         << objcoord.x << ", "
         << objcoord.y << ", "
         << objcoord.z << ")" << endl;

    wincoord.z = 1.0f;
    objcoord = glm::unProject( wincoord, camera.getTransformationMatrix(), projection, viewport );

    cout << "Coordinates in object space (zFar): ("
         << objcoord.x << ", "
         << objcoord.y << ", "
         << objcoord.z << ")" << endl;


    //comoApp->


    float ndcX, ndcY;

    if( mousePressEvent->button() == Qt::LeftButton ){
        cout << "Mouse press event" << endl
             << "\t Window coordinates: (" << mousePressEvent->x() << ", " << mousePressEvent->y() << ")" << endl;
        ndcX = ( mousePressEvent->x() - (width() >> 1) ) / (float)(width() >> 1 );
        ndcY = ( mousePressEvent->y() - (height() >> 1) ) / (float)(height() >> 1 );
        cout << "\t Normalized Device Coordinates: (" << ndcX << " ," << ndcY << ")" << endl;
    }

}

void OpenGLCanvas::keyPressEvent( QKeyEvent *e )
{
    cout << "Key press event" << endl;
    switch ( e->key() )
    {
        case Qt::Key_Left:
          //scene->translateSelectedDrawables( -0.01f, 0.0f, 0.0f );
          camera.translate( -0.01f, 0.0f, 0.0f );
        break;
        case Qt::Key_Right:
          //scene->translateSelectedDrawables( 0.01f, 0.0f, 0.0f );
          camera.translate( +0.01f, 0.0f, 0.0f );
        break;
        default:
          cout << "Unknown key (" << e->key() << ")" << endl;
        break;
    }
    render();
    //updateGL();
}

void OpenGLCanvas::mouseMoveEvent( QMouseEvent* mouseMoveEvent )
{
    static int lastX = mouseMoveEvent->x();
    static int lastY = mouseMoveEvent->y();

    //cout << "OpenGLCanvas::mouseMoveEvent (" << mouseMoveEvent->x()-lastX << ", " << mouseMoveEvent->y()-lastY << ")" << endl;

    //camera.rotateLaterally( mouseMoveEvent->x()-lastX );
    //camera.rotate( mouseMoveEvent->x()-lastX, 0.0f, 1.0f, 0.0f );

    lastX = mouseMoveEvent->x();
    lastY = mouseMoveEvent->y();

    render();
}


/***
 * 3. Updating and drawing
 ***/

void OpenGLCanvas::render()
{
    //cout << "OpenGLCanvas - Rendering" << endl;

    // Make shared OpenGL context current for this surface.
    comoApp->getOpenGLContext()->makeCurrent( this );

    // Make viewport occuppy the full canvas.
    glViewport( 0, 0, width(), height() );

    // Clear buffers.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Draw scene.
    comoApp->getScene()->draw( &camera );

    // Swap buffers.
    comoApp->getOpenGLContext()->swapBuffers( this );
}
