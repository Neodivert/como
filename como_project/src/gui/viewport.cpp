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

#include "viewport.hpp"
#include <cmath>
#include <iostream>
#include <glm/gtx/vector_angle.hpp>
using namespace std;

namespace como {


ProjectionModes projectionModes =
{
    Projection::ORTHO,
    Projection::PERSPECTIVE
};

ProjectionModeStrings projectionModeStrings =
{
    QString::fromUtf8( "Ortho" ),
    QString::fromUtf8( "Perspective" )
};


GLint Viewport::viewProjectionMatrixLocation = -1;


void Viewport::recordLastMousePos( const int& x, const int& y )
{
    lastMouseX = x;
    lastMouseY = y;
}


/***
 * 1. Initialization and destruction
 ***/

Viewport::Viewport( shared_ptr< ComoApp > comoApp ) :
    QWindow()
{
    GLint currentShaderProgram;

    // Make this canvas share the given app's state.
    this->comoApp = comoApp;

    // We will render using OpenGL.
    setSurfaceType( QWindow::OpenGLSurface );

    // Set this surface to the same format used by the app's shared OpenGL context.
    setFormat( comoApp->getOpenGLContext()->format() );
    destroy();
    create();

    comoApp->getOpenGLContext()->makeCurrent( this );

    cout << "Viewport 0" << endl;
    showError();

    // Compute dimensions' inverses.
    if( width() ){
        widthInverse = 1.0f / width();
    }
    if( height() ){
        heightInverse = 1.0f / height();
    }

    cout << "Viewport 1" << endl;
    showError();

    // Get location of uniform shader modelview matrix.
    if( viewProjectionMatrixLocation == -1 ){
        // Get current shader program id.
        glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

        viewProjectionMatrixLocation = glGetUniformLocation( currentShaderProgram, "viewProjectionMatrix" );
        cout << "viewProjectionMatrixLocation: (" << viewProjectionMatrixLocation << ")" << endl;
    }

    // Set default projection.
    setProjection( Projection::ORTHO );
}


/***
 * 2. Events
 ***/

bool Viewport::event(QEvent *event)
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


void Viewport::exposeEvent( QExposeEvent *event )
{
    // We are not interested in using the argument "event". The following macro prevents the
    // compiler from giving us a warning.
    Q_UNUSED(event);

    // Render this viewport if it's exposed.
    if ( isExposed() ){
        render();
    }
}


void Viewport::resizeEvent(QResizeEvent *event)
{
    // We are not interested in using the argument "event". The following macro prevents the
    // compiler from giving us a warning.
    Q_UNUSED(event);

    // Render this viewport if it's exposed.
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


void Viewport::mousePressEvent( QMouseEvent* mousePressEvent )
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
        //recordLastMousePos( mousePressEvent->x(), mousePressEvent->y() );
        lastMousePos = glm::vec2( mousePressEvent->x() * widthInverse, mousePressEvent->y() * heightInverse );

        // http://en.wikibooks.org/wiki/OpenGL_Programming/Object_selection

        // Get this canvas' viewport limits.
        viewport = glm::vec4( 0, 0, width(), height() );

        // Get window coordinates. Set z to near plane's z.
        windowCoordinates = glm::vec3( mousePressEvent->x(), height() - mousePressEvent->y() - 1, 0.0f );

        // Get world coordinates in clipping near plane by unproyecting the window's ones.
        worldCoordinates[0] = glm::unProject( windowCoordinates, camera.getViewMatrix(), projectionMatrix, viewport );

        // Get world coordinates in far clipping plane by unproyecting the window's ones.
        windowCoordinates.z = 1.0f;
        worldCoordinates[1] = glm::unProject( windowCoordinates, camera.getViewMatrix(), projectionMatrix, viewport );

        for( int i=0; i<2; i++ ){
            cout << "World coordinates (" << i << "): (";
            for( int j=0; j<3; j++ ){
                cout << worldCoordinates[i][j] << ", ";
            }
            cout << endl;
        }

        cout << "direction: (" << worldCoordinates[1][0]-worldCoordinates[0][0] << ", "
                               << worldCoordinates[1][1]-worldCoordinates[0][1] << ", "
                               << worldCoordinates[1][2]-worldCoordinates[0][2] << ")" << endl;

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


void Viewport::keyPressEvent( QKeyEvent *e )
{
    switch ( e->key() )
    {
        case Qt::Key_T:
            comoApp->setTransformationType( TransformationType::TRANSLATION );
            comoApp->setTransformationMode( TransformationMode::FREE );
        break;
        case Qt::Key_R:
            comoApp->setTransformationType( TransformationType::ROTATION );
            comoApp->setTransformationMode( TransformationMode::FREE );
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
        case Qt::Key_Left:
            comoApp->getScene()->rotateSelection( 90.0f, glm::vec3( 0.0f, 0.0f, 1.0f ) );
        break;
        case Qt::Key_Right:
            comoApp->getScene()->rotateSelection( -90.0f, glm::vec3( 0.0f, 0.0f, 1.0f ) );
        break;
        case Qt::Key_Delete:
            comoApp->getScene()->deleteSelection();
        break;
        case Qt::Key_0:
            setView( View::FRONT );
        break;
        case Qt::Key_1:
            setView( View::LEFT );
        break;
        case Qt::Key_2:
            setView( View::TOP );
        break;
        default:
          cout << "Unknown key (" << e->key() << ")" << endl;
        break;
    }
}


void Viewport::mouseMoveEvent( QMouseEvent* mouseMoveEvent )
{
    // Reference axis used in rotations.
    const glm::vec3 xAxis( 1.0f, 0.0f, 0.0f );
    const glm::vec3 yAxis( 0.0f, 1.0f, 0.0f );
    const glm::vec3 zAxis( 0.0f, 0.0f, 1.0f );

    // Compuse mouse pos (window normalized coordinates).
    const glm::vec2 mousePos = glm::vec2( mouseMoveEvent->pos().x() * widthInverse, mouseMoveEvent->pos().y() * heightInverse );

    // Compute the magnitude of the transformation.
    glm::vec4 translationVector;
    float angle;

    // Get current app's transformation type and mode.
    TransformationType transformationType = comoApp->getTransformationType();
    TransformationMode transformationMode = comoApp->getTransformationMode();

    // Only transform the scene when user is in Object mode.
    if( comoApp->getAppMode() == AppMode::OBJECT ){
        // Make the transformation requested by user.
        switch( transformationType ){
            case TransformationType::TRANSLATION:
                // Transform mouse move to a world space's translation vector.
                translationVector = 2.0f * glm::inverse( projectionMatrix*camera.getViewMatrix() ) * glm::vec4( mousePos.x - lastMousePos.x, lastMousePos.y - mousePos.y, 0.0f, 1.0f );

                // If requested, attach the translation vector to an axis.
                switch( transformationMode ){
                    case TransformationMode::FIXED_X:
                        translationVector.y = 0.0f;
                        translationVector.z = 0.0f;
                    break;
                    case TransformationMode::FIXED_Y:
                        translationVector.x = 0.0f;
                        translationVector.z = 0.0f;
                    break;
                    case TransformationMode::FIXED_Z:
                        translationVector.x = 0.0f;
                        translationVector.y = 0.0f;
                    break;
                    default:
                    break;
                }

                // Do the translation.
                comoApp->getScene()->translateSelection( glm::vec3( translationVector ) );
            break;
            case TransformationType::ROTATION:
                angle = 2.0f * glm::angle( glm::normalize( lastMousePos ), glm::normalize( mousePos ) );
                cout << "Angle: " << angle << endl;
                switch( transformationMode ){
                    case TransformationMode::FIXED_X:
                        comoApp->getScene()->rotateSelection( angle, xAxis );
                    break;
                    case TransformationMode::FIXED_Y:
                        comoApp->getScene()->rotateSelection( angle, yAxis );
                    break;
                    case TransformationMode::FIXED_Z:
                    case TransformationMode::FREE: // TODO: Change.
                        comoApp->getScene()->rotateSelection( angle, zAxis );
                    break;
                }
            break;
            case TransformationType::SCALE:
                cout << "Scale not implemented" << endl;
            break;
            default:
            break;
        }
    }

    // Record last mouse position.
    lastMousePos = mousePos;
}


/***
 * 3. Updating and drawing
 ***/

void Viewport::sendViewProjectionMatrixToShader( const glm::mat4& vpMatrix ) const
{
    glUniformMatrix4fv( viewProjectionMatrixLocation, 1, GL_FALSE, &vpMatrix[0][0] );
}

void Viewport::render()
{
    TransformationModes::iterator it;

    const glm::mat4 viewMatrix = camera.getViewMatrix();

    // Make shared OpenGL context current for this surface.
    comoApp->getOpenGLContext()->makeCurrent( this );

    // Make viewport occuppy the full canvas.
    glViewport( 0, 0, width(), height() );

    // Clear buffers.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Send view-projection matrix to shader.
    // TODO: use real viewProjection matrix.
    sendViewProjectionMatrixToShader( projectionMatrix*viewMatrix );

    // Draw scene.
    // TODO: move the find call to RenderPanel::render(). All the viewports will share
    // the result.
    it = find( transformationModes.begin(), transformationModes.end(), comoApp->getTransformationMode() );
    comoApp->getScene()->draw( std::distance( transformationModes.begin(), it ) - 1 );

    // Swap buffers.
    comoApp->getOpenGLContext()->swapBuffers( this );
}

/***
 * 5. Slots
 ***/

void Viewport::setView( View view )
{
    Views::iterator viewsIterator;

    camera.setView( view );
    comoApp->getScene()->renderNeeded();

    // Get the integer index of the current View on a array of views and return it
    // in a signal.
    viewsIterator = find( views.begin(), views.end(), view );
    emit viewIndexChanged( std::distance( views.begin(), viewsIterator ) );
}


void Viewport::setProjection( Projection projection )
{
    // TODO: Make both projections share some connection.
    switch( projection ){
        case Projection::ORTHO:
            projectionMatrix = glm::ortho( -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f );
        break;
        case Projection::PERSPECTIVE:
            projectionMatrix = glm::perspective( 60.0f, (float)(width())/(float)(height()), 0.1f, 1.0f );
        break;
    }

    //comoApp->getScene()->renderNeeded();
}

} // namespace como
