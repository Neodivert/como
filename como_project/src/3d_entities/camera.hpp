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
    ORTHO_LEFT = 0,
    ORTHO_RIGHT,
    ORTHO_BOTTOM,
    ORTHO_TOP,
    ORTHO_Z_NEAR,
    ORTHO_Z_FAR
};

enum PerspectiveAttribute {
    PERSPECTIVE_FOVY = 0,
    PERSPECTIVE_ASPECT,
    PERSPECTIVE_Z_NEAR,
    PERSPECTIVE_Z_FAR
};

const int N_PROJECTION_ATTRIBUTES = 6;

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

        // Locations for shader uniform variables.
        GLint viewMatrixLocation;
        GLint projectionMatrixLocation;

    public:
        /***
         * 1. Initializations
         ***/
        Camera();

        /***
         * 3.
         ***/
        void getViewMatrix();
        void setShaderProjectionMatrix();

        /***
         * 2. Projections
         ***/
        int setOrtho( float left, float right,
                      float bottom, float top,
                      float zNear, float zFar );
        int setPerspective( float fovy, float aspect,
                            float zNear, float zFar );

    protected:
        /***
         * 3. Updating and drawing
         ***/
        virtual void update();
    public:
        virtual void draw();
};

#endif // CAMERA_HPP
