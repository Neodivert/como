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
        const glm::vec4 originalEye;
        const glm::vec4 originalCenter;
        const glm::vec4 originalUp;

        // Camera transformed position and orientation.
        glm::vec4 transformedEye;
        glm::vec4 transformedCenter;
        glm::vec4 transformedUp;

        // View matrix.
        //

        // If true, camera is using perspective projection. Otherwise, orthographic projection
        // is being used.
        bool perspective;

        // If perspective == false, each element has a meaning according to OrthoAttribute enum.
        // Otherwise, each element has a meaning according to PerspectiveAttribute enum.
        float projectionAttributes[N_PROJECTION_ATTRIBUTES];

        // Projection matrix.
        glm::mat4 projectionMatrix;

        // Locations for shader uniform variables.
        static GLint modelviewMatrixLocation;
        static GLint projectionMatrixLocation;


        //glm::mat4 vieeeewMatrix;

    public:
        /***
         * 1. Initializations
         ***/
        Camera();


        void rotateLaterally( float angle );

        virtual void intersects( glm::vec3 r0, glm::vec3 r1, float& t, unsigned int* triangle = nullptr  ) const
        {
            Q_UNUSED( r0 ); Q_UNUSED( r1 ); t = -1.0f; Q_UNUSED( triangle );
        }

        /***
         * 3.
         ***/
        void setShaderModelviewMatrix( const glm::mat4* modelMatrix );
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
        virtual void draw( Camera* camera, bool selected = false ) const ;
};

#endif // CAMERA_HPP
