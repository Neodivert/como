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

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include "drawable_entity.hpp"

enum OrthoAttribute {
    LEFT = 0,
    RIGHT,
    BOTTOM,
    TOP
};

enum PerspectiveAttribute {
    FOVY = 0,
    ASPECT,
    Z_NEAR,
    Z_FAR
};

const int N_PROJECTION_ATTRIBUTES = 4;

class Camera : public DrawableEntity
{
    protected:
        // Camera original position and orientation.
        const glm::vec3 originalEye;
        const glm::vec3 originalCenter;
        const glm::vec3 originalUp;

        // Camera transformed position and orientation.
        glm::vec3 transformedEye;
        glm::vec3 transformedCenter;
        glm::vec3 transformedUp;

        // If true, camera is using perspective projection. Otherwise, orthographic projection
        // is being used.
        bool perspective;

        // If perspective == false, each element has a meaning according to OrthoAttribute enum.
        // Otherwise, each element has a meaning according to PerspectiveAttribute enum.
        float projectionAttributes[N_PROJECTION_ATTRIBUTES];

        // Projection matrix.
        glm::mat4 projectionMatrix;

    public:
        Camera();
};

#endif // CAMERA_HPP
