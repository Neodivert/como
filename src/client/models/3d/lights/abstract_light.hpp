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

#ifndef ABSTRACT_LIGHT_HPP
#define ABSTRACT_LIGHT_HPP

#include <common/packables/array/packable_color.hpp>

namespace como {

class AbstractLight
{
    public:
        /***
         * 1. Construction
         ***/
        AbstractLight() = default;
        AbstractLight( const AbstractLight& ) = default;
        AbstractLight( AbstractLight&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~AbstractLight() = default;


        /***
         * 3. Getters
         ***/
        virtual PackableColor getLightColor() const = 0;
        virtual float getAmbientCoefficient() const = 0;


        /***
         * 4. Setters
         ***/
        virtual void setLightColor( const PackableColor& color ) = 0;
        virtual void setAmbientCoefficient( float coefficient ) = 0;


        /***
         * 5. Operators
         ***/
        AbstractLight& operator = ( const AbstractLight& ) = default;
        AbstractLight& operator = ( AbstractLight&& ) = default;
};

} // namespace como

#endif // ABSTRACT_LIGHT_HPP
