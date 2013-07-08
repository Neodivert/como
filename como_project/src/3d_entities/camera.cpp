#include "camera.hpp"

Camera::Camera() :
    // Initialize original camera position and orientation.
    // TODO: It would better if I make these attributes static (and the same with Cube's
    // original vertices)?
    originalEye( 0.0f, 0.0f, 0.0f ),
    originalCenter( 0.0f, 0.0f, -1.0f ),
    originalUp( 0.0f, 1.0f, 0.0f )
{
    /*
    originalEye = glm::vec3( 0.0f, 0.0f, 0.0f );
    originalCenter = glm::vec3( 0.0f, 0.0f, -1.0f );
    originalUp = glm::vec3( 0.0f, 1.0f, 0.0f );
    */
}
