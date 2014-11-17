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

#ifndef ABSTRACT_MATERIAL_HPP
#define ABSTRACT_MATERIAL_HPP

#include <common/utilities/color.hpp>

namespace como {

class AbstractMaterial
{
    public:
        /***
         * 1. Getters
         ***/
        virtual Color getColor() const = 0;
        virtual Color getAmbientReflectivity() const = 0;
        virtual Color getDiffuseReflectivity() const = 0;
        virtual Color getSpecularReflectivity() const = 0;
        virtual float getSpecularExponent() const = 0;


        /***
         * 2. Setters
         ***/
        virtual void setColor( const Color& color ) = 0;
        virtual void setAmbientReflectivity( const Color& ambientReflectivity ) = 0;
        virtual void setDiffuseReflectivity( const Color& diffuseReflectivity ) = 0;
        virtual void setSpecularReflectivity( const Color& specularReflectivity ) = 0;
        virtual void setSpecularExponent( float specularExponent ) = 0;
};


} // namespace como

#endif // ABSTRACT_MATERIAL_HPP
