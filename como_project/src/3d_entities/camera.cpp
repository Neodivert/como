#include "camera.hpp"

GLint Camera::modelviewMatrixLocation = -1;
GLint Camera::projectionMatrixLocation = -1;

Camera::Camera() :
    // Initialize original camera position and orientation.
    // TODO: It would better if I make these attributes static?
    originalEye( 0.0f, 0.0f, 0.0f, 1.0f ),
    originalCenter( 0.0f, 0.0f, -1.0f, 1.0f ),
    originalUp( 0.0f, 1.0f, 0.0f, 1.0f )
{
    update();

    /*
    cout << "transformedEye: (" << transformedEye[X] << ", " << transformedEye[Y] << ", " << transformedEye[Z] << ")" << endl;
    cout << "transformedCenter: (" << transformedCenter[X] << ", " << transformedCenter[Y] << ", " << transformedCenter[Z] << ")" << endl;
    cout << "transformedUp: (" << transformedUp[X] << ", " << transformedUp[Y] << ", " << transformedUp[Z] << ")" << endl;
    */

    GLint currentShaderProgram;

    // Get current shader program id.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

    // Get location of uniform shader modelview matrix.
    if( modelviewMatrixLocation == -1 ){
        modelviewMatrixLocation = glGetUniformLocation( currentShaderProgram, "modelviewMatrix" );
    }
    cout << "modelviewMatrixLocation: (" << modelviewMatrixLocation << ")" << endl;

    /*
    // Get location of uniform shader projection matrix.
    if( projectionMatrixLocation == -1 ){
        projectionMatrixLocation = glGetUniformLocation( currentShaderProgram, "projectionMatrix" );
    }
    cout << "projectionMatrixLocation: (" << projectionMatrixLocation << ")" << endl;
    */
}


void Camera::rotateLaterally( float angle )
{
    cout << "Rotating Laterally " << angle << " degrees around (" << transformedUp[X] << ", " << transformedUp[Y] << ", " << transformedUp[Z] << ")" << endl;
    glm::rotate( transformationMatrix, angle, glm::vec3( transformedUp ) );

    update();
}


/***
 *
 ***/

void Camera::setShaderModelviewMatrix( const glm::mat4* modelMatrix )
{
    glm::mat4 viewMatrix;

    viewMatrix = glm::lookAt( glm::vec3( transformedEye ),
                              glm::vec3( transformedCenter ),
                              glm::vec3( transformedUp ) );


    // Compute modelview matrix.
    glm::mat4 modelviewMatrix = transformationMatrix * (*modelMatrix);

    // Copy computed modelview matrix to its corresponding shader uniform.
    glUniformMatrix4fv( modelviewMatrixLocation, 1, GL_FALSE, &modelviewMatrix[0][0] );
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
    }else{
        return -1;
    }
}


/***
 * Updating and drawing
 ***/

void Camera::update()
{
    // Update transformed camera position and orientation.
    transformedEye = transformationMatrix * originalEye;
    transformedCenter = transformationMatrix * originalCenter;
    transformedUp = transformationMatrix * originalUp;

    // Update view matrix with transformed camera's position and orientation.
    glm::mat4 viewMatrix;

    viewMatrix = glm::lookAt( glm::vec3( transformedEye ),
                              glm::vec3( transformedCenter ),
                              glm::vec3( transformedUp ) );

    cout << "transformationMatrix: ";
    for( int i=0; i<4; i++ ){
        for( int j=0; j<4; j++ ){
            cout << transformationMatrix[i][j] << ", ";
        }
        cout << endl;
    }


    cout << "viewMatrix: ";
    for( int i=0; i<4; i++ ){
        for( int j=0; j<4; j++ ){
            cout << viewMatrix[i][j] << ", ";
        }
        cout << endl;
    }
}


void Camera::draw( Camera* camera, bool selected ) const
{
    // TODO: When ready, make Camera inherit from Geometry and delete this.
    Q_UNUSED( camera );
    Q_UNUSED( selected );
}
