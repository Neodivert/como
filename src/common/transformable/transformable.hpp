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

#ifndef TRANSFORMABLE_HPP
#define TRANSFORMABLE_HPP

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl.h>

namespace como {

class Transformable
{
    public:
        /***
         * 1. Construction
         ***/
        Transformable() = default;
        Transformable( const Transformable& ) = default;
        Transformable( Transformable&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~Transformable() = default;


        /***
         * 3. Getters
         ***/
        virtual glm::vec3 centroid() const = 0;


        /***
         * 4. Transformations
         ***/
        virtual void translate( glm::vec3 direction );

        void rotateAroundOrigin( GLfloat angle, glm::vec3 axis );
        void rotateAroundCentroid( GLfloat angle, const glm::vec3& axis );
        void rotateAroundPivot( const GLfloat& angle, const glm::vec3& axis, const glm::vec3& pivot );

        void scaleAroundOrigin( const glm::vec3& scaleFactors );
        void scaleAroundCentroid( const glm::vec3& scaleFactors );
        void scaleAroundPivot( const glm::vec3& scaleFactors, const glm::vec3& pivot );

        virtual void applyTransformationMatrix( const glm::mat4& transformation ) = 0;
        virtual void setModelMatrix( const glm::mat4& modelMatrix ) = 0;


        /***
         * 5. Operators
         ***/
        Transformable& operator = ( const Transformable& ) = default;
        Transformable& operator = ( Transformable&& ) = default;
};


}

#endif // TRANSFORMABLE_HPP
