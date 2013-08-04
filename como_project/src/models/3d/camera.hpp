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
#include <glm/gtc/matrix_transform.hpp>
#include "mesh.hpp"
#include "cube.hpp"

namespace como {

/*
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
*/

/*
const int N_PROJECTION_ATTRIBUTES = 6;
*/

enum class View {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    FRONT,
    BACK,
    CAMERA
};


class Camera : public Mesh
{
    protected:
        // Camera's original position and orientation.
        const glm::vec4 originalEye;
        const glm::vec4 originalUp;
        const glm::vec4 originalCenter;

        // Camera's transformed position and orientation.
        glm::vec4 transformedEye;
        glm::vec4 transformedUp;
        glm::vec4 transformedCenter;

    public:
        /***
         * 1. Initializations
         ***/
        Camera();


        /***
         * 2. Setters and getters
         ***/
        glm::mat4 getViewMatrix() const ;
        void setView( View view );

        /***
         * 2. Projections
         ***/
        /*
        int setOrtho( float left, float right,
                      float bottom, float top,
                      float zNear, float zFar );
        int setPerspective( float fovy, float aspect,
                            float zNear, float zFar );
        */
    protected:
        /***
         * 3. Updating and drawing
         ***/
        virtual void update();
};

} // namespace como

#endif // CAMERA_HPP
