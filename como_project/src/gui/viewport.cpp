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

GLuint Viewport::guideRectVAO = 0;
GLuint Viewport::guideRectVBO = 0;

ProjectionModes projectionModes =
{{
    Projection::ORTHO,
    Projection::PERSPECTIVE
}};

ProjectionModeStrings projectionModeStrings =
{{
    QString::fromUtf8( "Ortho" ),
    QString::fromUtf8( "Perspective" )
}};


GLint Viewport::viewProjectionMatrixLocation = -1;


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

    // Initialize the guide rect's VAO and VBO.
    if( !guideRectVAO ){
        initGuideRect();
    }

    focus = false;
}

Viewport::~Viewport()
{
    glDeleteVertexArrays( 1, &guideRectVAO );
    glDeleteBuffers( 1, &guideRectVBO );
}

void Viewport::initGuideRect()
{
    const GLfloat guideRect[] {
        -0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    // Create a VAO for the guide rect.
    glGenVertexArrays( 1, &guideRectVAO );
    glBindVertexArray( guideRectVAO );

    // Create a VBO for the guide rect.
    glGenBuffers( 1, &guideRectVBO );
    glBindBuffer( GL_ARRAY_BUFFER, guideRectVBO );
    glBufferData( GL_ARRAY_BUFFER, 6*sizeof( GLfloat ), guideRect, GL_DYNAMIC_DRAW );

    // TODO: Use real vPosition.
    GLuint vPosition = 0;
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( vPosition );
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
            comoApp->getScene()->rotateSelection( 90.0f, glm::vec3( 0.0f, 0.0f, 1.0f ), comoApp->getPivotPointMode() );
        break;
        case Qt::Key_Right:
            comoApp->getScene()->rotateSelection( -90.0f, glm::vec3( 0.0f, 0.0f, 1.0f ), comoApp->getPivotPointMode() );
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

    // Variables used for computing the magnitude of the transformation.
    glm::vec4 transformVector;
    float angle;
    //glm::vec4 pivotPoint( 0.0f );

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
                transformVector = 2.0f * glm::inverse( projectionMatrix*camera.getViewMatrix() ) * glm::vec4( mousePos.x - lastMousePos.x, lastMousePos.y - mousePos.y, 0.0f, 1.0f );

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
                // Compute the angle between the vectors mousePos and lastMousePos.
                angle = glm::orientedAngle(glm::normalize( mousePos ), glm::normalize( lastMousePos ) );

                // Make the rotation about an axis or another depending on the current
                // transformationMode.
                switch( transformationMode ){
                    case TransformationMode::FIXED_X:
                        comoApp->getScene()->rotateSelection( angle, xAxis, comoApp->getPivotPointMode() );
                    break;
                    case TransformationMode::FIXED_Y:
                        comoApp->getScene()->rotateSelection( angle, yAxis, comoApp->getPivotPointMode() );
                    break;
                    case TransformationMode::FIXED_Z:
                        comoApp->getScene()->rotateSelection( angle, zAxis, comoApp->getPivotPointMode() );
                    break;
                    case TransformationMode::FREE:
                        comoApp->getScene()->rotateSelection( angle, glm::vec3( -camera.getCenterVector() ), comoApp->getPivotPointMode() );
                    break;
                }

                updateGuideRect( mouseMoveEvent->pos().x(), height() - mouseMoveEvent->pos().y() - 1 );
            break;
            case TransformationType::SCALE:
                // Compute the scale magnitud.
                transformVector = glm::vec4( mousePos.x / lastMousePos.x, mousePos.y / lastMousePos.y, 1.0f, 1.0f );

                // Transform the scale vector from window to world space.
                transformVector = Drawable::transformScaleVector( transformVector, glm::inverse( projectionMatrix * camera.getViewMatrix() ) );

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

                // Do the scale.
                comoApp->getScene()->scaleSelection( glm::vec3( transformVector ) );
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
    sendViewProjectionMatrixToShader( projectionMatrix*viewMatrix );

    // Draw scene.
    // TODO: move the find call to RenderPanel::render(). All the viewports will share
    // the result.
    it = find( transformationModes.begin(), transformationModes.end(), comoApp->getTransformationMode() );
    comoApp->getScene()->draw( std::distance( transformationModes.begin(), it ) - 1 );

    // Draw guide rect
    if( ( comoApp->getTransformationType() == TransformationType::ROTATION ) ||
        ( comoApp->getTransformationType() == TransformationType::SCALE ) ){
        // TODO: Draw only if we have mouse focus.
        renderGuideRect();
    }

    // Make viewport occupy the bottom left corner.
    glViewport( width()-50, 0, 50, 50 );

    // Draw scene's world axis.
    glDisable( GL_DEPTH_TEST );
    comoApp->getScene()->drawWorldAxis();
    glEnable( GL_DEPTH_TEST );

    // Swap buffers.
    comoApp->getOpenGLContext()->swapBuffers( this );
}


void Viewport::renderGuideRect()
{
    glBindVertexArray( guideRectVAO );
    glBindBuffer( GL_ARRAY_BUFFER, guideRectVBO );

    glDisable( GL_DEPTH_TEST );
    glDrawArrays( GL_LINES, 0, 2 );
    glEnable( GL_DEPTH_TEST );
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
            projectionMatrix = glm::ortho( -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f );
        break;
        case Projection::PERSPECTIVE:
            projectionMatrix = glm::perspective( 60.0f, (float)(width())/(float)(height()), 0.1f, 1.0f );
        break;
    }

    //comoApp->getScene()->renderNeeded();
}


/***
 * 6. Auxiliar methods
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
    rayOrigin = glm::unProject( windowCoordinates, camera.getViewMatrix(), projectionMatrix, viewport );

    // Get ray direction coordinates by unproyecting the window's ones to far plane and
    // then substracting the ray origin.
    windowCoordinates.z = 1.0f;
    rayDirection = glm::unProject( windowCoordinates, camera.getViewMatrix(), projectionMatrix, viewport ) - rayOrigin;
}

void Viewport::updateGuideRect( const GLfloat& x, const GLfloat& y )
{
    glm::vec3 rayOrigin, rayDestiny;
    GLfloat t;
    glm::vec3 destination;
    GLfloat* guideRectBuffer = nullptr;
    unsigned int i = 0;

    // The scene's current pivot point will be the rect's origin.
    glm::vec3 origin = comoApp->getScene()->getPivotPoint( comoApp->getPivotPointMode() );

    // Get the equation of a plane which contains the origin point and whose normal
    // is the opposite of the camera's center vector.
    const GLfloat A = ( -camera.getCenterVector() ).x;
    const GLfloat B = ( -camera.getCenterVector() ).y;
    const GLfloat C = ( -camera.getCenterVector() ).z;
    const GLfloat D = -A * origin.x - B * origin.y - C * origin.z;

    // Trace a ray from pixel (x, y) towards camera center vector. Get the the ray's
    // origin (at zNear plane) and direction.
    traceRay( x, y, rayOrigin, rayDestiny );

    // Get the intersection between the previous ray and plane.
    t = -( ( A * rayOrigin.x + B * rayOrigin.y + C * rayOrigin.z + D ) / ( A * rayDestiny.x + B * rayDestiny.y + C * rayDestiny.z ) );

    // Use the previous intersection parameter t for computing the guide rect's destination.
    destination = rayOrigin + rayDestiny * t;

    // Map the guide rect VBO to client memory and update it with the newest
    // origin and destination.
    glBindBuffer( GL_ARRAY_BUFFER, guideRectVBO );
    guideRectBuffer = (GLfloat *)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );

    for( ; i<3; i++ ){
        guideRectBuffer[i] = origin[i];
    }
    for( ; i<6; i++ ){
        guideRectBuffer[i] = destination[i-3];
    }

    glUnmapBuffer( GL_ARRAY_BUFFER );
}


} // namespace como
