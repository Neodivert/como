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

#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <client/models/utilities/open_gl.hpp>

namespace como {

class Drawable
{
    public:
        /***
         * 1. Construction
         ***/
        Drawable() = default;
        Drawable( const Drawable& ) = default;
        Drawable( Drawable&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~Drawable() = default;


        /***
         * 3. Drawing
         ***/
        virtual void draw( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec4* contourColor = nullptr ) const = 0;


        /***
         * 4. Operators
         ***/
        Drawable& operator = ( const Drawable& ) = default;
        Drawable& operator = ( Drawable&& ) = default;
};

}

#endif // DRAWABLE_HPP
