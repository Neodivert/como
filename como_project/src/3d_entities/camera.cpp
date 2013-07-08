#include "camera.hpp"

Camera::Camera() :
    // Initialize original camera position and orientation.
    // TODO: It would better if I make these attributes static?
    originalEye( 0.0f, 0.0f, 0.0f ),
    originalCenter( 0.0f, 0.0f, -1.0f ),
    originalUp( 0.0f, 1.0f, 0.0f )
{
    /*
    GLint currentShaderProgram;


    // Get current shader program id.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

    // Get location of uniform shader variable "color".
    uniformColorLocation = glGetUniformLocation( currentShaderProgram, "color" );
    */
}


/***
 *
 ***/

void Camera::getViewMatrix()
{

}


void Camera::setShaderProjectionMatrix()
{

}


/***
 * Projections
 ***/

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
    }
}

/***
 * Updating and drawing
 ***/

void Camera::update()
{
    // Update transformed camera position and orientation.
    transformedEye = transformationMatrix * vec4( originalEye );
    transformedCenter = transformationMatrix * vec4( originalCenter );
    transformedUp = transformationMatrix * vec4( originalUp );
}

void Camera::draw()
{

}
