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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <client/models/dependencies.hpp>

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

enum class DrawableType : std::uint8_t
{
    MESH = 0
};


/***
 * Main file class
 ***/

std::string getOpenGLErrorString( GLenum errorCode );
void checkOpenGL( const char* str );

class Drawable
{
    private:
        /*! Type of drawable */
        const DrawableType type_;

        /*! Name given to the drawable */
        std::string name_;

    protected:
        std::array< glm::vec4, 3 > originalOrientation;
        std::array< glm::vec4, 3 > transformedOrientation;

        glm::mat4 transformationMatrix;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        // Constructor. Initialize transformation matrix to identity matrix.
        Drawable( DrawableType type, std::string name );
        Drawable( const Drawable& ) = delete;
        Drawable( Drawable&& ) = delete;

        virtual ~Drawable(){}


        /***
         * 2. Getters
         ***/
        glm::mat4 getTransformationMatrix();
        virtual glm::vec4 getCentroid() const = 0;
        DrawableType getType() const;
        std::string getName() const;


        /***
         * 3. Transformations
         ***/
        virtual void translate( glm::vec3 direction );

        virtual void rotate( GLfloat angle, glm::vec3 axis );
        virtual void rotate( const GLfloat& angle, glm::vec3 axis, const glm::vec3& pivot );

        virtual void scale( glm::vec3 scaleFactors );
        virtual void scale( glm::vec3 scaleFactors, const glm::vec3& pivot );


        virtual void intersects( glm::vec3 r0, glm::vec3 r1, float& t, unsigned int* triangle = nullptr  ) const = 0;


        /***
         * 4. Selection
         ***/
        void select();
        void unselect();


        /***
         * 5. Operators
         ***/
        Drawable& operator=( const Drawable& ) = delete ;
        Drawable& operator=( Drawable&& ) = delete;


        /***
         * 6. Updating and drawing
         ***/
    protected:
        virtual void update();

    public:
        virtual void draw( const glm::mat4& viewProjMatrix, const GLfloat* contourColor = nullptr ) const = 0;

        /***
         * 7. Auxiliar methods
         ***/
        static glm::vec4 transformScaleVector( glm::vec4 scaleVector, const glm::mat4& transformMatrix );
};

typedef std::shared_ptr< Drawable > DrawablePtr;
typedef std::shared_ptr< const Drawable > DrawableConstPtr;

} // namespace como

#endif // DRAWABLE_ENTITY_HPP
