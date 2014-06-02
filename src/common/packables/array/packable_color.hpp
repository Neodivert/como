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

#ifndef PACKABLE_COLOR_HPP
#define PACKABLE_COLOR_HPP

#include <common/packables/array/packable_array.hpp>
#include <common/packables/packable_uint8.hpp>
#include <common/packables/array/packable_color.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace como {

class PackableColor : public PackableArray< PackableUint8< std::uint8_t >, std::uint8_t, 4 >
{
    public:
        /***
         * 1. Construction
         ***/
        PackableColor();
        PackableColor( const glm::vec3& channels );
        PackableColor( const glm::vec4& channels );
        PackableColor( const std::uint8_t* channels );
        PackableColor( const std::array< std::uint8_t, 4>& channels );
        PackableColor( std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255 );
        PackableColor( const PackableColor& ) = default;
        PackableColor( PackableColor&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~PackableColor() = default;


        /***
         * 3. Conversions
         ***/
        glm::vec3 toVec3() const ;
        glm::vec4 toVec4() const ;


        /***
         * 4. Operators
         ***/
        PackableColor& operator = ( const PackableColor& ) = default;
        PackableColor& operator = ( PackableColor&& ) = default;
} const DEFAULT_PACKABLE_COLOR;

} // namespace como

#endif // PACKABLE_COLOR_HPP
