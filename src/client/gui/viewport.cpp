/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "viewport.hpp"
#include <cmath>
#include <glm/gtx/vector_angle.hpp>

namespace como {


GLint Viewport::viewProjectionMatrixLocation = -1;


/***
 * 1. Initialization and destruction
 ***/

Viewport::Viewport( View view, shared_ptr< ComoApp > comoApp ) :
    QWindow()
{
    try {
        GLint currentShaderProgram;

        // Make this canvas share the given app's state.
        this->comoApp = comoApp;

        // We will render using OpenGL.
        setSurfaceType( QWindow::OpenGLSurface );

        // Set this surface to the same format used by the app's shared OpenGL context.
        setFormat( comoApp->getScene()->getOpenGLContext()->format() );
        destroy();
        create();

        comoApp->getScene()->getOpenGLContext()->makeCurrent( this );

        checkOpenGL( "Viewport constructor, before creating the camera" );

        // Get location of uniform shader modelview matrix.
        if( viewProjectionMatrixLocation == -1 ){
            // Get current shader program id.
            glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

            viewProjectionMatrixLocation = glGetUniformLocation( currentShaderProgram, "viewProjectionMatrix" );
        }

        // Create the camera.
        camera = new Camera( view );

        checkOpenGL( "Viewport constructor, after creating the camera" );

        //checkOpenGL( "Viewport constructor, before computing dimension inverses" );

        // Compute dimensions' inverses.
        if( width() ){
            widthInverse = 1.0f / width();
        }
        if( height() ){
            heightInverse = 1.0f / height();
        }

        // Set default projection.
        setProjection( Projection::ORTHO );

        checkOpenGL( "Viewport constructor - end" );
    }catch( std::exception& ex ){
        comoApp->getLog()->error( ex.what() );
        throw ex;
    }
}


Viewport::~Viewport()
{
    delete camera;
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
    glm::vec3 rayOrigin, rayDirection;
    bool addToSelection = false;

    // Do one thing or another depending on which transformation mode we are in.
    if( comoApp->getTransformationType() == TransformationType::NONE ){
        // We are not in transformation mode. This mouse press is for selecting
        // a drawable.

        // Record last mouse position.
        //recordLastMousePos( mousePressEvent->x(), mousePressEvent->y() );
        lastMousePos = getNormalizedMousePos( mousePressEvent->x(), mousePressEvent->y() );

        // http://en.wikibooks.org/wiki/OpenGL_Programming/Object_selection
        // Tace a ray from pixel towards camera's center vector.
        traceRay( mousePressEvent->x(), height() - mousePressEvent->y() - 1, rayOrigin, rayDirection );

        // If user is pressing ctrl key while selecting a drawable, add it to the current
        // selection. Otherwise, clear the current selection and select the new drawable.
        addToSelection = mousePressEvent->modifiers() & Qt::ControlModifier;

        // Do the ray picking.
        comoApp->getScene()->selectDrawableByRayPicking( rayOrigin,
                                                         rayDirection,
                                                         addToSelection );
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
        case Qt::Key_S:
            comoApp->setTransformationType( TransformationType::SCALE );
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
        break;
    }
}


void Viewport::mouseMoveEvent( QMouseEvent* mouseMoveEvent )
{
    // Reference axis used in rotations.
    const glm::vec3 xAxis( 1.0f, 0.0f, 0.0f );
    const glm::vec3 yAxis( 0.0f, 1.0f, 0.0f );
    const glm::vec3 zAxis( 0.0f, 0.0f, 1.0f );

    // Variables used for computing the magnitude of the transformation.
    glm::vec4 transformVector;
    float angle;
    // TODO: Not a complete conversion to screen coordinates.
    glm::vec2 scenePivotPoint = glm::vec2( projectionMatrix * camera->getViewMatrix() * glm::vec4( comoApp->getScene()->getPivotPoint(), 1.0f ) );
    scenePivotPoint.x *= 0.5f;
    scenePivotPoint.y *= -0.5f;

    glm::vec2 pivotPointToMousePosVector;
    glm::vec2 lastPivotPointToMousePosVector;

    // Compute mouse pos (window normalized coordinates [-0.5, 0.5]).
    const glm::vec2 mousePos = getNormalizedMousePos( mouseMoveEvent->pos().x(), mouseMoveEvent->pos().y() );

    // Get current app's transformation type and mode.
    TransformationType transformationType = comoApp->getTransformationType();
    TransformationMode transformationMode = comoApp->getTransformationMode();

    // Only transform the scene when user is in Object mode.
    if( comoApp->getAppMode() == AppMode::OBJECT ){
        // Make the transformation requested by user.
        switch( transformationType ){
            case TransformationType::TRANSLATION:
                // Transform mouse move to a world space's translation vector.
                transformVector = 2.0f * glm::inverse( projectionMatrix*camera->getViewMatrix() ) * glm::vec4( mousePos.x - lastMousePos.x, lastMousePos.y - mousePos.y, 0.0f, 1.0f );

                // If requested, attach the translation vector to an axis.
                switch( transformationMode ){
                    case TransformationMode::FIXED_X:
                        transformVector.y = 0.0f;
                        transformVector.z = 0.0f;
                    break;
                    case TransformationMode::FIXED_Y:
                        transformVector.x = 0.0f;
                        transformVector.z = 0.0f;
                    break;
                    case TransformationMode::FIXED_Z:
                        transformVector.x = 0.0f;
                        transformVector.y = 0.0f;
                    break;
                    default:
                    break;
                }

                // Do the translation.
                comoApp->getScene()->translateSelection( glm::vec3( transformVector ) );
            break;
            case TransformationType::ROTATION:
                // Compute the angle between the vectors.
                pivotPointToMousePosVector = glm::normalize( mousePos - scenePivotPoint );
                lastPivotPointToMousePosVector = lastMousePos - scenePivotPoint;

                // Error checking : if lastPivotPointToMousePosVector's lenght
                // would be zero, we would have a NaN when normalizing it.
                if( glm::length( lastPivotPointToMousePosVector ) != 0.0f ){
                    lastPivotPointToMousePosVector = glm::normalize( lastPivotPointToMousePosVector );

                    angle = glm::orientedAngle( pivotPointToMousePosVector, lastPivotPointToMousePosVector );

                    // Make the rotation about an axis or another depending on the current
                    // transformationMode.
                    switch( transformationMode ){
                        case TransformationMode::FIXED_X:
                            comoApp->getScene()->rotateSelection( angle, xAxis );
                        break;
                        case TransformationMode::FIXED_Y:
                            comoApp->getScene()->rotateSelection( angle, yAxis );
                        break;
                        case TransformationMode::FIXED_Z:
                            comoApp->getScene()->rotateSelection( angle, zAxis );
                        break;
                        case TransformationMode::FREE:
                            comoApp->getScene()->rotateSelection( angle, glm::vec3( -camera->getCenterVector() ) );
                        break;
                    }
                }

            break;
            case TransformationType::SCALE:
                // Compute the scale magnitud.
                pivotPointToMousePosVector = mousePos - scenePivotPoint;
                lastPivotPointToMousePosVector = lastMousePos - scenePivotPoint;

                // Error checking : if lastPivotPointToMousePosVector's
                // components would be zero, we would have a NaN when
                // dividing by it.
                if( lastPivotPointToMousePosVector.x != 0.0f &&
                    lastPivotPointToMousePosVector.y != 0.0f ){
                    transformVector = glm::vec4( pivotPointToMousePosVector.x / lastPivotPointToMousePosVector.x,
                                                 pivotPointToMousePosVector.y / lastPivotPointToMousePosVector.y,
                                                 1.0f,
                                                 1.0f );

                    // Transform the scale vector from window to world space.
                    transformVector = Drawable::transformScaleVector( transformVector, glm::inverse( projectionMatrix * camera->getViewMatrix() ) );

                    // If requested, attach the tranformation vector to an axis.
                    switch( transformationMode ){
                        case TransformationMode::FIXED_X:
                            transformVector.y = 1.0f;
                            transformVector.z = 1.0f;
                        break;
                        case TransformationMode::FIXED_Y:
                            transformVector.x = 1.0f;
                            transformVector.z = 1.0f;
                        break;
                        case TransformationMode::FIXED_Z:
                            transformVector.x = 1.0f;
                            transformVector.y = 1.0f;
                        break;
                        default:
                        break;
                    }

                    // Do the scale only if none of the scaling factors is
                    // zero.
                    if( transformVector.x != 0.0f &&
                        transformVector.y != 0.0f &&
                        transformVector.z != 0.0f ){
                        // Do the scale.
                        comoApp->getScene()->scaleSelection( glm::vec3( transformVector ) );
                    }
                }
            break;
            default:
            break;
        }
    }

    // Update the transform guide line.
    updateTransformGuideLine( mouseMoveEvent->pos().x(), height() - mouseMoveEvent->pos().y() - 1 );

    // Record last mouse position.
    lastMousePos = mousePos;
}


/***
 * 3. Updating and drawing
 ***/

void Viewport::render()
{
    // Don't render if surface is not exposed.
    if( !isExposed() ){
        return;
    }

    // Get camera's view matrix.
    glm::mat4 viewMatrix = camera->getViewMatrix();

    // Make shared OpenGL context current for this surface.
    comoApp->getScene()->getOpenGLContext()->makeCurrent( this );

    // Make viewport occuppy the full canvas.
    glViewport( 0, 0, width(), height() );

    // Clear buffers.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Draw scene.
    comoApp->getScene()->draw( projectionMatrix*viewMatrix, static_cast< int >( comoApp->getTransformationMode() ) - 1 );

    // Now we'll draw some objets that are already in world space, so we update
    // MVP Matrix with a identity model matrix.
    Mesh::sendMVPMatrixToShader( projectionMatrix*viewMatrix );

    // Draw guide rect
    if( ( comoApp->getTransformationType() == TransformationType::ROTATION ) ||
        ( comoApp->getTransformationType() == TransformationType::SCALE ) ){
        comoApp->getScene()->drawTransformGuideLine();
    }

    // Make viewport occupy the bottom left corner.
    glViewport( width()-50, 0, 50, 50 );

    // Draw scene's world axis.
    glDisable( GL_DEPTH_TEST );
    comoApp->getScene()->drawWorldAxis();
    glEnable( GL_DEPTH_TEST );

    // Swap buffers.
    comoApp->getScene()->getOpenGLContext()->swapBuffers( this );
}


/***
 * 6. Slots
 ***/

void Viewport::setView( View view )
{
    comoApp->getScene()->getOpenGLContext()->makeCurrent( this );

    camera->setView( view );
    comoApp->getScene()->renderNeeded();

    // Get the integer index of the current View and return it
    // in a signal.
    emit viewIndexChanged( static_cast< int >( view ) );
}


void Viewport::setProjection( Projection projection )
{
    // TODO: Make both projections share some connection.
    switch( projection ){
        case Projection::ORTHO:
            projectionMatrix = glm::ortho( -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f );
        break;
        case Projection::PERSPECTIVE:
            projectionMatrix = glm::perspective( 60.0f, (float)(width())/(float)(height()), 0.1f, 1.0f );
        break;
    }

    //comoApp->getScene()->renderNeeded();
}


/***
 * 7. Auxiliar methods
 ***/

glm::vec2 Viewport::getNormalizedMousePos( const int& x, const int& y ) const
{
    return glm::vec2( (x * widthInverse) - 0.5f, (y * heightInverse) - 0.5f );
}


void Viewport::traceRay( const GLfloat& x, const GLfloat& y, glm::vec3& rayOrigin, glm::vec3& rayDirection ) const
{
    glm::vec4 viewport;
    glm::vec3 windowCoordinates;

    // Get this canvas' viewport limits.
    viewport = glm::vec4( 0, 0, width(), height() );

    // Get window coordinates. Set z to near plane's z.
    windowCoordinates = glm::vec3( x, y, 0.0f );

    // Get ray origin coordinates at clipping near plane by unproyecting the window's ones.
    rayOrigin = glm::unProject( windowCoordinates, camera->getViewMatrix(), projectionMatrix, viewport );

    // Get ray direction coordinates by unproyecting the window's ones to far plane and
    // then substracting the ray origin.
    windowCoordinates.z = 1.0f;
    rayDirection = glm::unProject( windowCoordinates, camera->getViewMatrix(), projectionMatrix, viewport ) - rayOrigin;
}


void Viewport::updateTransformGuideLine( const GLfloat& x, const GLfloat& y )
{
    glm::vec3 rayOrigin, rayDestination;
    GLfloat t;
    glm::vec3 destination;

    // The scene's current pivot point will be the rect's origin.
    glm::vec3 origin = comoApp->getScene()->getPivotPoint();

    // Get the equation of a plane which contains the origin point and whose normal
    // is the opposite of the camera's center vector.
    const GLfloat A = ( -camera->getCenterVector() ).x;
    const GLfloat B = ( -camera->getCenterVector() ).y;
    const GLfloat C = ( -camera->getCenterVector() ).z;
    const GLfloat D = -A * origin.x - B * origin.y - C * origin.z;

    // Trace a ray from pixel (x, y) towards camera center vector. Get the the ray's
    // origin (at zNear plane) and direction.
    traceRay( x, y, rayOrigin, rayDestination );

    // Get the intersection between the previous ray and plane.
    t = -( ( A * rayOrigin.x + B * rayOrigin.y + C * rayOrigin.z + D ) / ( A * rayDestination.x + B * rayDestination.y + C * rayDestination.z ) );

    // Use the previous intersection parameter t for computing the guide rect's destination.
    destination = rayOrigin + rayDestination * t;

    // Update the transform guide line with the new origin and destination.
    comoApp->getScene()->setTransformGuideLine( origin, destination );
}


} // namespace como
