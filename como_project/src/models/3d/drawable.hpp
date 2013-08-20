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

#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP


/****
 * Includes
 ***/

#include <iostream>
using namespace std;

//#include <GL/glew.h>

// http://www.opengl.org/discussion_boards/showthread.php/172481-glGenBuffer-was-not-declared
#define GL_GLEXT_PROTOTYPES
#include <QtOpenGL>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>

namespace como {


/***
 * Enum types
 ***/
enum COORDINATES
{
    X = 0,
    Y,
    Z
};


/***
 * Main file class
 ***/

void showError();

class Drawable
{
    protected:
        std::array< glm::vec4, 3 > originalOrientation;
        std::array< glm::vec4, 3 > transformedOrientation;

        glm::mat4 rotationMatrix;
        glm::mat4 rotationAndScaleMatrix;


        glm::vec3 scaleVector;

        glm::mat4 transformationMatrix;

    public:
        /***
         * 1. Initialization
         ***/
        // Constructor. Initialize transformation matrix to identity matrix.
        Drawable();

        /***
         * 2. Setters and getters
         ***/
        glm::mat4 getTransformationMatrix();
        virtual glm::vec4 getCentroid() const = 0;

        /***
         * 3. Transformations
         ***/
        glm::vec3 transformToObjectSpace( const glm::vec3 worldVector );

        virtual void translate( glm::vec3 direction );

        virtual void rotate( GLfloat angle, glm::vec3 axis );

        virtual void scale( glm::vec3 scaleFactors );
        //virtual void rotate( const GLfloat& angle, const glm::vec3& axis, const glm::vec3& pivot );

        virtual void intersects( glm::vec3 r0, glm::vec3 r1, float& t, unsigned int* triangle = nullptr  ) const = 0;


        /***
         * 4. Selection
         ***/
        void select();
        void unselect();

        /***
         * 5. Updating and drawing
         ***/
    protected:
        virtual void update();

    public:
        virtual void draw( bool selected = false ) const = 0;

        /***
         * 6. Auxiliar methods
         ***/
        static glm::vec4 transformScaleVector( glm::vec4 scaleVector, const glm::mat4& transformMatrix );
};

} // namespace como

#endif // DRAWABLE_ENTITY_HPP
