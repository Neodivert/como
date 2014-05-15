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

#include "color.hpp"

namespace como {


/***
 * 1. Construction
 ***/

Color::Color( std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a )
{
    channels_[0] = r;
    channels_[1] = g;
    channels_[2] = b;
    channels_[4] = a;
}


Color::Color( const QColor& qtColor )
{
    channels_[0] = static_cast< std::uint8_t >( qtColor.red() );
    channels_[1] = static_cast< std::uint8_t >( qtColor.green() );
    channels_[2] = static_cast< std::uint8_t >( qtColor.blue() );
    channels_[3] = static_cast< std::uint8_t >( qtColor.alpha() );
}


Color::Color( glm::vec4 floatColor )
{
    unsigned int i = 0;

    for( i = 0; i < N_COLOR_CHANNELS; i++ ){
        channels_[i] = static_cast< std::uint8_t >( floatColor[i] * 255 );
    }
}


/***
 * 3. Type transformation
 ***/

glm::vec4 Color::toVec4()
{
    unsigned int i = 0;
    glm::vec4 floatColor;

    for( i = 0; i < N_COLOR_CHANNELS; i++ ){
        floatColor[i] = channels_[i] / 255.0f;
    }

    return floatColor;
}


/***
 * 4. Channel access
 ***/

const std::uint8_t& Color::operator [] ( const unsigned int i ) const
{
    return channels_.at( i );
}

std::uint8_t& Color::operator [] ( const unsigned int i )
{
    return channels_.at( i );
}


/***
 * 5. Operators
 ***/

bool Color::operator == ( const Color& b )
{
    unsigned int i = 0;

    for( i = 0; i < N_COLOR_CHANNELS; i++ ){
        if( channels_[i] != b.channels_[i] ){
            return false;
        }
    }

    return true;
}

} // namespace como
