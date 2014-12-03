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

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>     // glm::lookAt
#include <glm/gtc/type_ptr.hpp>             // glm::value_ptr

namespace como {

GLint Camera::eyeVectorLocation_ = -1;

Camera::Camera( const ResourceID& id, OpenGL &openGL, const glm::vec3 &cameraCenter, const glm::vec3 &cameraEye, const glm::vec3 &cameraUp ) :
    ImportedMesh( id, "data/system/primitives/camera.prim" ),
    originalEye     ( 0.0f, 0.0f, 1.0f, 1.0f ),
    originalUp      ( 0.0f, 1.0f, 0.0f, 0.0f ),
    originalCenter  ( 0.0f, 0.0f, 0.0f, 1.0f )
{
    if( eyeVectorLocation_ < 0 ){
        eyeVectorLocation_ = openGL.getShaderVariableLocation( "eyeVector" );
    }

    setOrientation( cameraEye, cameraCenter, cameraUp );
}


/***
 * 2. Setters and getters
 ***/

glm::mat4 Camera::getViewMatrix() const
{
    return viewMatrix;
}


glm::vec4 Camera::getCenterPosition() const
{
    return transformedCenter;
}


glm::vec4 Camera::getCenterVector() const
{
    return transformedCenter - transformedEye;
}


glm::vec3 Camera::getEye() const
{
    return glm::vec3( transformedEye );
}


glm::vec3 Camera::getUpVector() const
{
    return glm::vec3( transformedUp );
}


std::string Camera::typeName() const
{
    return "Camera";
}


/***
 * 3. Setters
 ***/

void Camera::setOrientation( const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up )
{
    viewMatrix = glm::lookAt( eye, center, up );
    modelMatrix_ = glm::inverse( viewMatrix );

    update();
}


/***
 * 4. Shader communication
 ***/

void Camera::sendToShader( OpenGL &openGL ) const
{
    glm::vec3 eyeVector = glm::vec3( transformedEye - transformedCenter );
    if( eyeVector.length() != 0.0f ){
        eyeVector = glm::normalize( eyeVector );
    }
    openGL.setUniformVec3( eyeVectorLocation_, eyeVector );
}


/***
 * 6. Updating and drawing
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
