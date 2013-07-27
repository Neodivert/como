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


namespace como {

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

    // Compute dimensions' inverses.
    if( width() ){
        widthInverse = 1.0f / width();
    }
    if( height() ){
        heightInverse = 1.0f / height();
    }
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

    // Recompute the inverses of canvas' dimensions.
    if( width() ){
        widthInverse = 1.0f / width();
    }
    if( height() ){
        heightInverse = 1.0f / height();
    }
}


void OpenGLCanvas::mousePressEvent( QMouseEvent* mousePressEvent )
{
    glm::vec4 viewport;
    glm::vec3 windowCoordinates;
    glm::mat4 projectionMatrix;
    glm::vec3 worldCoordinates[2];
    bool addToSelection = false;

    // Do one thing or another depending on which transformation mode we are in.
    if( comoApp->getTransformationType() == TransformationType::NONE ){
        // We are not in transformation mode. This mouse press is for selecting
        // a drawable.

        // Record last mouse position.
        lastMouseX = mousePressEvent->x();
        lastMouseY = mousePressEvent->y();

        // http://en.wikibooks.org/wiki/OpenGL_Programming/Object_selection

        // Get this canvas' viewport limits.
        viewport = glm::vec4( 0, 0, width(), height() );

        // Get window coordinates. Set z to near plane's z.
        windowCoordinates = glm::vec3( mousePressEvent->x(), height() - mousePressEvent->y() - 1, 0.0f );
        //windowCoordinates = glm::vec3( mousePressEvent->x(), mousePressEvent->y(), 0.0f );

        // Get projection matrix (TODO: In future versions, get the camera's one).
        projectionMatrix = glm::ortho( -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f );

        // Get world coordinates in clipping near plane by unproyecting the window's ones.
        worldCoordinates[0] = glm::unProject( windowCoordinates, camera.getTransformationMatrix(), projectionMatrix, viewport );

        // Get world coordinates in far clipping plane by unproyecting the window's ones.
        windowCoordinates.z = 1.0f;
        worldCoordinates[1] = glm::unProject( windowCoordinates, camera.getTransformationMatrix(), projectionMatrix, viewport );

        // If user is pressing ctrl key while selecting a drawable, add it to the current
        // selection. Otherwise, clear the current selection and select the new drawable.
        addToSelection = mousePressEvent->modifiers() & Qt::ControlModifier;

        // Do the ray picking.
        comoApp->getScene()->selectDrawableByRayPicking( worldCoordinates[0],
                                                        worldCoordinates[1] - worldCoordinates[0], addToSelection );
    }else{
        // We were in transformation mode. This mouse press is for droping
        // the current selection.
        comoApp->setTransformationType( TransformationType::NONE );
    }
}

void OpenGLCanvas::keyPressEvent( QKeyEvent *e )
{
    switch ( e->key() )
    {
        case Qt::Key_T:
            //comoApp->setAppMode( AppMode::EDITION );
            comoApp->setTransformationType( TransformationType::TRANSLATION );
            comoApp->setTransformationMode( TransformationMode::FREE );
            //comoApp->getScene()->translateSelectedDrawables( -0.01f, 0.0f, 0.0f );
            //camera.translate( -0.01f, 0.0f, 0.0f );
        break;
        case Qt::Key_R:
            //comoApp->setAppMode( AppMode::EDITION );
            comoApp->setTransformationType( TransformationType::ROTATION );
            comoApp->setTransformationMode( TransformationMode::FREE );
            //comoApp->getScene()->translateSelectedDrawables( 0.01f, 0.0f, 0.0f );
            //camera.translate( +0.01f, 0.0f, 0.0f );
        break;
        case Qt::Key_X:
            comoApp->setTransformationMode( TransformationMode::FIXED_X );
        break;
        case Qt::Key_Y:
            comoApp->setTransformationMode( TransformationMode::FIXED_Y );
        break;
        case Qt::Key_Z:
            comoApp->setTransformationMode( TransformationMode::FIXED_Z );
        break;
        case Qt::Key_Tab:
            // Swap between Object and Edition modes.
            if( comoApp->getAppMode() == AppMode::OBJECT ){
                comoApp->setAppMode( AppMode::EDITION );
            }else{
                comoApp->setAppMode( AppMode::OBJECT );
            }
        break;
        case Qt::Key_Delete:
            comoApp->getScene()->deleteSelection();
        break;
        default:
          cout << "Unknown key (" << e->key() << ")" << endl;
        break;
    }
}


void OpenGLCanvas::mouseMoveEvent( QMouseEvent* mouseMoveEvent )
{
    float tx, ty;
    TransformationMode transformationMode = comoApp->getTransformationMode();

    // Only transform the scene when user is holding mouse left button and
    // he/she in edition mode.
    //if( ( mouseMoveEvent->buttons() & Qt::LeftButton )
    if( comoApp->getAppMode() == AppMode::OBJECT ){
        // Compute the magnitude of the transformation.
        tx = ( mouseMoveEvent->x() - lastMouseX ) * widthInverse;
        ty = ( lastMouseY - mouseMoveEvent->y() ) * heightInverse;

        // Truncate values if the current transformation mode is not the appropiate.
        tx = ( ( transformationMode == TransformationMode::FREE ) || ( transformationMode == TransformationMode::FIXED_X ) ) * tx;
        ty = ( ( transformationMode == TransformationMode::FREE ) || ( transformationMode == TransformationMode::FIXED_Y ) ) * ty;

        // Make the transformation requested by user.
        switch( comoApp->getTransformationType() ){
            case TransformationType::TRANSLATION:
                comoApp->getScene()->translateSelectedDrawables( tx, ty, 0.0f );
            break;
            case TransformationType::ROTATION:
                comoApp->getScene()->rotateSelectedDrawables( 100*tx, 0.0f, 1.0f, 0.0f );
                comoApp->getScene()->rotateSelectedDrawables( 100*ty, 1.0f, 0.0f, 0.0f );
            break;
            case TransformationType::SCALE:
                cout << "Scale not implemented" << endl;
            break;
            default:
            break;
        }
    }

    // Record last mouse position.
    lastMouseX = mouseMoveEvent->x();
    lastMouseY = mouseMoveEvent->y();
}


/***
 * 3. Updating and drawing
 ***/

void OpenGLCanvas::render()
{
    const glm::mat4 projectionMatrix = glm::ortho( -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f );
    //const glm::mat4 viewMatrix = glm::mat4( 1.0f );
    const glm::mat4 viewMatrix = glm::lookAt( glm::vec3( 0.0f, 0.0f, 1.0f ),
                                              glm::vec3( 0.0f, 0.0f, -1.0f ),
                                              glm::vec3( 0.0f, 1.0f, 0.0f ) );

    // Make shared OpenGL context current for this surface.
    comoApp->getOpenGLContext()->makeCurrent( this );

    // Make viewport occuppy the full canvas.
    glViewport( 0, 0, width(), height() );

    // Clear buffers.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Draw scene.
    // TODO: use real viewProjection matrix.
    comoApp->getScene()->draw( projectionMatrix*viewMatrix );

    // Swap buffers.
    comoApp->getOpenGLContext()->swapBuffers( this );
}

} // namespace como
