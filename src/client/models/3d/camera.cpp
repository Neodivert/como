#include "camera.hpp"

namespace como {

const PackableColor DEFAULT_CAMERA_MESH_COLOR( 255, 255, 255, 0 );

Camera::Camera( View view ) :
    Mesh( MeshType::CAMERA, "data/system/primitives/camera.prim", MaterialConstPtr( new Material ) ), // TODO: Load material from file.
    originalEye     ( 0.0f, 0.0f, 0.0f, 1.0f ),
    originalUp      ( 0.0f, 1.0f, 0.0f, 1.0f ),
    originalCenter  ( 0.0f, 0.0f, -1.0f, 1.0f )
{
    // Set given view.
    setView( view );
}


DrawablePtr Camera::clone()
{
    return DrawablePtr( new Camera( *this ) );
}


/***
 * 2. Setters and getters
 ***/


glm::mat4 Camera::getViewMatrix() const
{
    return viewMatrix;
}


void Camera::setView( View view )
{
    const glm::vec3 X_AXIS = glm::vec3( 1.0f, 0.0f, 0.0f );
    const glm::vec3 Y_AXIS = glm::vec3( 0.0f, 1.0f, 0.0f );
    const float PI = glm::pi<float>();
    const float HALF_PI = glm::half_pi<float>();

    OpenGL::checkStatus( "Camera::setView - 1" );

    switch( view ){
        case View::LEFT:
            transformationMatrix = glm::rotate( glm::mat4( 1.0f ), HALF_PI, Y_AXIS );
        break;
        case View::RIGHT:
            transformationMatrix = glm::rotate( glm::mat4( 1.0f ), -HALF_PI, Y_AXIS );
        break;
        case View::TOP:
            transformationMatrix = glm::rotate( glm::mat4( 1.0f ), HALF_PI, X_AXIS );
        break;
        case View::BOTTOM:
            transformationMatrix = glm::rotate( glm::mat4( 1.0f ), -HALF_PI, X_AXIS );
        break;
        case View::FRONT:
            transformationMatrix = glm::mat4( 1.0f );
        break;
        case View::BACK:
            transformationMatrix = glm::rotate( glm::mat4( 1.0f ), PI, Y_AXIS );
        break;
        case View::CAMERA:
        break;
    }

    OpenGL::checkStatus( "Camera::setView - 2" );
    update();

    OpenGL::checkStatus( "Camera::setView - 3" );
}


glm::vec4 Camera::getCenterVector() const
{
    return transformedCenter;
}


/***
 * 4. Updating and drawing
 ***/

void Camera::update()
{
    // Update camera's mesh.
    // FIXME: What mesh? I didn't define camera's vertex data yet.
    Mesh::update();

    // Update transformed camera position and orientation.
    transformedEye = transformationMatrix * originalEye;
    transformedUp = transformationMatrix * originalUp;
    transformedCenter = transformationMatrix * originalCenter;

    // Update view matrix.
    viewMatrix = glm::inverse( transformationMatrix );
}


} // namespace como
