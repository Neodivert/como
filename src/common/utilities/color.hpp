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

#ifndef COLOR_HPP
#define COLOR_HPP

#include <array>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace como {

class Color
{
    public:
        /***
         * 1. Construction
         ***/
        Color();
        Color( const glm::vec3& channels );
        Color( const glm::vec4& channels );
        Color( const std::uint8_t* channels );
        Color( const std::array< std::uint8_t, 4>& channels );
        Color( std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255 );
        Color( const Color& ) = default;
        Color( Color&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~Color() = default;


        /***
         * 3. Getters
         ***/
        std::array< std::uint8_t, 4> channels() const;
        glm::vec3 toVec3() const;
        glm::vec4 toVec4() const;


        /***
         * 4. Operators
         ***/
        Color& operator = ( const Color& ) = default;
        Color& operator = ( Color&& ) = default;
        Color& operator = ( const glm::vec3& channels );
        Color& operator = ( const glm::vec4& channels );
        Color& operator = ( const std::uint8_t* channels );
        Color& operator = ( const std::array< std::uint8_t, 4>& channels );
        const std::uint8_t& operator [] ( unsigned int index ) const;
        std::uint8_t& operator [] ( unsigned int index );
        bool operator == ( const Color& ) const;
        bool operator != ( const Color& ) const;



    private:
        std::array< std::uint8_t, 4 > channels_;
};

} // namespace color

#endif // COLOR_HPP
