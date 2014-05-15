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

#ifndef COLOR_HPP
#define COLOR_HPP

#include <array>
#include <cinttypes>
#include <QColor>
#include <glm/glm.hpp>
#include <stdexcept>

namespace como {

const unsigned int N_COLOR_CHANNELS = 4;

class Color
{
    private:
        std::array< std::uint8_t, 4 > channels_; // RGBA

    public:
        /***
         * 1. Construction
         ***/
        Color( std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255 );
        Color( const QColor& qtColor );
        Color( glm::vec4 floatColor );
        Color( const Color& ) = default;
        Color( Color&& ) = default;


        /***
         * 2. Destructor
         ***/
        ~Color() = default;


        /***
         * 3. Type transformation
         ***/
        glm::vec4 toVec4();


        /***
         * 4. Channel access
         ***/
        const std::uint8_t& operator [] ( const unsigned int i ) const;
        std::uint8_t& operator [] ( const unsigned int i );


        /***
         * 5. Operators
         ***/
        Color& operator = ( const Color& ) = default;
        Color& operator = ( Color&& ) = default;

        bool operator == ( const Color& b );
};

} // namespace como

#endif // COLOR_HPP
