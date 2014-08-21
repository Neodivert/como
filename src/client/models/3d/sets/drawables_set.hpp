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

#ifndef DRAWABLES_SET_HPP
#define DRAWABLES_SET_HPP

#include <client/models/3d/drawable.hpp>

namespace como {

class DrawablesSet
{
    public:
        /***
         * 1. Construction
         ***/
        DrawablesSet( const glm::vec4& borderColor = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
        DrawablesSet() = delete;
        DrawablesSet( const DrawablesSet& ) = default;
        DrawablesSet( DrawablesSet&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~DrawablesSet() = default;


        /***
         * 3. Getters
         ***/
        glm::vec4 borderColor() const;


        /***
         * 4. Setters
         ***/
        void setBorderColor( const glm::vec4& color );


        /***
         * 5. Drawing
         ***/
        virtual void draw( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const = 0;


        /***
         * 6. Operators
         ***/
        DrawablesSet& operator = ( const DrawablesSet& ) = default;
        DrawablesSet& operator = ( DrawablesSet&& ) = default;


    private:
        glm::vec4 borderColor_;
};

} // namespace como

#endif // DRAWABLES_SET_HPP
