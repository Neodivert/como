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
#include <glm/gtc/type_ptr.hpp>

namespace como {

GLint Camera::eyeVectorLocation_ = -1;

Camera::Camera( OpenGL& openGL, View view ) :
    ImportedMesh( "data/system/primitives/camera.prim" ),
    originalEye     ( 0.0f, 0.0f, 1.0f, 1.0f ),
    originalUp      ( 0.0f, 1.0f, 0.0f, 0.0f ),
    originalCenter  ( 0.0f, 0.0f, 0.0f, 1.0f )
{
    if( eyeVectorLocation_ < 0 ){
        eyeVectorLocation_ = openGL.getShaderVariableLocation( "eyeVector" );
    }

    // Set given view.
    setView( view );
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
    glm::mat4 viewMatrix = glm::mat4( 1.0f );

    const glm::vec3 center( 0.0f, 0.0f, 0.0f );

    switch( view ){
        case View::LEFT:
            viewMatrix = glm::lookAt(
                        glm::vec3( -1.0f, 0.0f, 0.0f ),
                        center,
                        glm::vec3( 0.0f, 1.0f, 0.0f )
                        );
        break;
        case View::RIGHT:
            viewMatrix = glm::lookAt(
                        glm::vec3( 1.0f, 0.0f, 0.0f ),
                        center,
                        glm::vec3( 0.0f, 1.0f, 0.0f )
                        );
        break;
        case View::TOP:
            viewMatrix = glm::lookAt(
                        glm::vec3( 0.0f, 1.0f, 0.0f ),
                        center,
                        glm::vec3( 0.0f, 0.0f, -1.0f )
                        );
        break;
        case View::BOTTOM:
            viewMatrix = glm::lookAt(
                        glm::vec3( 0.0f, -1.0f, 0.0f ),
                        center,
                        glm::vec3( 0.0f, 0.0f, 1.0f )
                        );
        break;
        case View::FRONT:
            viewMatrix = glm::lookAt(
                        glm::vec3( 0.0f, 0.0f, 1.0f ),
                        center,
                        glm::vec3( 0.0f, 1.0f, 0.0f )
                        );
        break;
        case View::BACK:
            viewMatrix = glm::lookAt(
                        glm::vec3( 0.0f, 0.0f, -1.0f ),
                        center,
                        glm::vec3( 0.0f, 1.0f, 0.0f )
                        );
        break;
        case View::CAMERA:
            viewMatrix = glm::lookAt(
                        glm::vec3( 1.0f, 1.0f, 1.0f ),
                        center,
                        glm::vec3( -1.0f, 1.0f, -1.0f )
                        );
        break;
    }

    modelMatrix_ = glm::inverse( viewMatrix );

    update();
}


glm::vec4 Camera::getCenterVector() const
{
    return transformedCenter;
}


/***
 * 3. Shader communication
 ***/

void Camera::sendToShader( OpenGL &openGL )
{
    // TODO: Use OpenGL class.
    (void)( openGL );

    const glm::vec3 eyeVector = glm::normalize( glm::vec3( transformedEye - transformedCenter ) );
    glUniform3fv( eyeVectorLocation_, 1, glm::value_ptr( eyeVector ) );
}


/***
 * 5. Updating and drawing
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
