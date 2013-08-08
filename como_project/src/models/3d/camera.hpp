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

// Array with all the possible app mode values (for conversion between
// View and int).
typedef std::array< View, N_VIEWS > Views;
extern Views views;

// Array with a string for each view value (for output in GUI).
typedef std::array< QString, N_VIEWS > ViewStrings;
extern ViewStrings viewStrings;



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

    protected:
        /***
         * 3. Updating and drawing
         ***/
        virtual void update();
};

} // namespace como

#endif // CAMERA_HPP
