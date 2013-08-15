#include "camera.hpp"

namespace como {

Views views =
{{
    View::FRONT,
    View::BACK,
    View::LEFT,
    View::RIGHT,
    View::TOP,
    View::BOTTOM,
    View::CAMERA
}};

ViewStrings viewStrings =
{{
    QString::fromUtf8( "Front view" ),
    QString::fromUtf8( "Back view" ),
    QString::fromUtf8( "Left view" ),
    QString::fromUtf8( "Right view" ),
    QString::fromUtf8( "Top view" ),
    QString::fromUtf8( "Bottom view" ),
    QString::fromUtf8( "Camera view" )
}};


Camera::Camera() :
    originalEye     ( 0.0f, 0.0f, 0.0f, 1.0f ),
    originalUp      ( 0.0f, 1.0f, 0.0f, 1.0f ),
    originalCenter  ( 0.0f, 0.0f, -1.0f, 1.0f )
{
    // Update camera's vertices and orientation.
    update();
}


/***
 * 2. Setters and getters
 ***/


glm::mat4 Camera::getViewMatrix() const
{
    return glm::inverse( transformationMatrix );
}
/*
void foo( char* str, glm::mat4 M )
{
    glm::vec4 eye = M * glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    glm::vec4 up = M * glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
    glm::vec4 center = M * glm::vec4( 0.0f, 0.0f, -1.0f, 1.0f );

    cout << str << endl
         << "---" << endl
         << "Transformed eye: (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << endl
         << "Transformed up (" << up.x << ", " << up.y << ", " << up.z << ")" << endl
         << "Transformed center (" << center.x << ", " << center.y << ", " << center.z << endl
         << "---" << endl;
}
*/
void Camera::setView( View view )
{
    const glm::vec3 X_AXIS = glm::vec3( 1.0f, 0.0f, 0.0f );
    const glm::vec3 Y_AXIS = glm::vec3( 0.0f, 1.0f, 0.0f );

    translationMatrix = glm::mat4( 1.0f );
    switch( view ){
        case View::LEFT:
            rotationMatrix = glm::rotate( glm::mat4( 1.0f ), -90.0f, Y_AXIS );
        break;
        case View::RIGHT:
            rotationMatrix = glm::rotate( glm::mat4( 1.0f ), 90.0f, Y_AXIS );
        break;
        case View::TOP:
            rotationMatrix = glm::rotate( glm::mat4( 1.0f ), -90.0f, X_AXIS );
        break;
        case View::BOTTOM:
            rotationMatrix = glm::rotate( glm::mat4( 1.0f ), 90.0f, X_AXIS );
        break;
        case View::FRONT:
            rotationMatrix = glm::mat4( 1.0f );
        break;
        case View::BACK:
            rotationMatrix = glm::rotate( glm::mat4( 1.0f ), 180.0f, Y_AXIS );
        break;
        case View::CAMERA:
        break;
    }

    transformationMatrix = rotationMatrix;

    update();
}


glm::vec4 Camera::getCenterVector() const
{
    return transformedCenter;
}

/***
 *
 ***/
/*
void Camera::setShaderModelviewMatrix( const glm::mat4* modelMatrix )
{
    glm::mat4 viewMatrix;

    viewMatrix = glm::lookAt( glm::vec3( transformedEye ),
                              glm::vec3( transformedOrientation[Z] ),
                              glm::vec3( transformedOrientation[Y] ) );


    // Compute modelview matrix.
    glm::mat4 modelviewMatrix = transformationMatrix * (*modelMatrix);

    // Copy computed modelview matrix to its corresponding shader uniform.
    glUniformMatrix4fv( modelviewMatrixLocation, 1, GL_FALSE, &modelviewMatrix[0][0] );
}
*

void Camera::setShaderProjectionMatrix()
{

}

*/
/***
 * Projections
 ***/

/*

int Camera::setOrtho( float left, float right,
                      float bottom, float top,
                      float zNear, float zFar )
{
    if( ( left != right ) && ( bottom != top ) && (zNear != zFar) ){
        perspective = false;

        projectionAttributes[ ORTHO_LEFT ] = left;
        projectionAttributes[ ORTHO_RIGHT ] = right;
        projectionAttributes[ ORTHO_BOTTOM ] = bottom;
        projectionAttributes[ ORTHO_TOP ] = top;
        projectionAttributes[ ORTHO_Z_NEAR ] = zNear;
        projectionAttributes[ ORTHO_Z_FAR ] = zFar;

        projectionMatrix = glm::ortho( left, right, bottom, top, zNear, zFar );

        return 0;
    }else{
        return -1;
    }
}


int Camera::setPerspective( float fovy, float aspect,
                            float zNear, float zFar )
{
    if( zNear != zFar ){
        perspective = true;

        projectionAttributes[ PERSPECTIVE_FOVY ] = fovy;
        projectionAttributes[ PERSPECTIVE_ASPECT ] = aspect;
        projectionAttributes[ PERSPECTIVE_Z_NEAR ] = zNear;
        projectionAttributes[ PERSPECTIVE_Z_FAR ] = zFar;

        projectionMatrix = glm::perspective( fovy, aspect, zNear, zFar );

        return 0;
    }else{
        return -1;
    }
}

*/

/***
 * Updating and drawing
 ***/

void Camera::update()
{
    // Update camera's mesh.
    Mesh::update();

    // Update transformed camera position and orientation.
    transformedEye = transformationMatrix * originalEye;
    transformedUp = transformationMatrix * originalUp;
    transformedCenter = transformationMatrix * originalCenter;
}


} // namespace como
