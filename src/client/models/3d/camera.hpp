/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
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

#include "mesh.hpp"

namespace como {

// Available views.
enum class View {
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    CAMERA
};
const unsigned int N_VIEWS = 7;

// Available views (strings for GUI output).
const char viewStrings [N_VIEWS][16] =
{
    "Front view",
    "Back view",
    "Left view",
    "Right view",
    "Top view",
    "Bottom view",
    "Camera view"
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

        // View matrix.
        glm::mat4 viewMatrix;

    public:
        /***
         * 1. Initializations
         ***/
        Camera( View view = View::FRONT );

        /***
         * 2. Setters and getters
         ***/
        glm::mat4 getViewMatrix() const ;
        void setView( View view );
        glm::vec4 getCenterVector() const ;

    protected:
        /***
         * 3. Updating and drawing
         ***/
        virtual void update();
};

} // namespace como

#endif // CAMERA_HPP
