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

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <client/models/3d/meshes/imported_mesh.hpp>

#include <glm/gtc/constants.hpp>

namespace como {

class Camera : public ImportedMesh
{
    private:
        // Camera's original position and orientation.
        const glm::vec4 originalEye;
        const glm::vec4 originalUp;
        const glm::vec4 originalCenter;

        // Camera's transformed position and orientation.
        glm::vec4 transformedEye;
        glm::vec4 transformedUp;
        glm::vec4 transformedCenter;

        // View matrix.
        glm::mat4 viewMatrix;

        //
        static GLint eyeVectorLocation_;

    public:
        /***
         * 1. Initializations
         ***/
        Camera( const Camera& ) = default;
        Camera( Camera&& ) = default;
        Camera( OpenGL& openGL,
                const glm::vec3& cameraCenter = glm::vec3( 0.0f, 0.0f, 0.0f ),
                const glm::vec3& cameraEye = glm::vec3( 0.0f, 0.0f, 1.0f ),
                const glm::vec3& cameraUp = glm::vec3( 0.0f, 1.0f, 0.0f ) );

        // TODO: Is this problematic with Entity's virtual destructor?
        virtual ~Camera() = default;

        /***
         * 2. Getters
         ***/
        glm::mat4 getViewMatrix() const ;
        glm::vec4 getCenterPosition() const ;
        glm::vec4 getCenterVector() const ;
        glm::vec3 getEye() const;
        glm::vec3 getUpVector() const;


        /***
         * 3. Setters
         ***/
        void setOrientation( const glm::vec3& eye,
                             const glm::vec3& center,
                             const glm::vec3& up );


        /***
         * 4. Shader communication
         ***/
        void sendToShader( OpenGL& openGL ) const;


        /***
         * 5. Operators
         ***/
        Camera& operator=( const Camera& ) = delete ;
        Camera& operator=( Camera&& ) = delete;

    protected:
        /***
         * 6. Updating and drawing
         ***/
        virtual void update();
};

} // namespace como

#endif // CAMERA_HPP
