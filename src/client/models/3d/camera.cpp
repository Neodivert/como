/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published by
 * the Free Software Foundation.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "camera.hpp"

namespace como {

const PackableColor DEFAULT_CAMERA_MESH_COLOR( 255, 255, 255, 0 );

Camera::Camera( View view ) :
    Mesh( MeshType::CAMERA, "data/system/primitives/camera.prim", MaterialConstPtr( new Material ) ), // TODO: Load material from file.
    originalEye     ( 0.0f, 0.0f, 0.0f, 0.0f ),
    originalUp      ( 0.0f, 1.0f, 0.0f, 0.0f ),
    originalCenter  ( 0.0f, 0.0f, -1.0f, 0.0f )
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

    switch( view ){
        case View::LEFT:
            modelMatrix_ = glm::rotate( glm::mat4( 1.0f ), HALF_PI, Y_AXIS );
        break;
        case View::RIGHT:
            modelMatrix_ = glm::rotate( glm::mat4( 1.0f ), -HALF_PI, Y_AXIS );
        break;
        case View::TOP:
            modelMatrix_ = glm::rotate( glm::mat4( 1.0f ), HALF_PI, X_AXIS );
        break;
        case View::BOTTOM:
            modelMatrix_ = glm::rotate( glm::mat4( 1.0f ), -HALF_PI, X_AXIS );
        break;
        case View::FRONT:
            modelMatrix_ = glm::mat4( 1.0f );
        break;
        case View::BACK:
            modelMatrix_ = glm::rotate( glm::mat4( 1.0f ), PI, Y_AXIS );
        break;
        case View::CAMERA:
            modelMatrix_ =
                    glm::rotate( glm::mat4( 1.0f ), -HALF_PI / 2.0f, Y_AXIS ) *
                    glm::rotate( glm::mat4( 1.0f ), HALF_PI / 2.0f, X_AXIS ) *
                    glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.0f, -1.5f ) );
        break;

    }

    update();
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
    Mesh::update();

    // Update transformed camera position and orientation.
    transformedEye = modelMatrix_ * originalEye;
    transformedUp = modelMatrix_ * originalUp;
    transformedCenter = modelMatrix_ * originalCenter;

    // Update view matrix.
    viewMatrix = glm::inverse( modelMatrix_ );
}


} // namespace como
