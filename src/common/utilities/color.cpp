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

#include "color.hpp"

namespace como {

/***
 * 1. Construction
 ***/

Color::Color() :
    channels_({ 0 })
{}


Color::Color( const glm::vec3 &channels )
{
    operator = ( channels );
}


Color::Color(const glm::vec4 &channels)
{
    operator = ( channels );
}


Color::Color(const std::uint8_t *channels)
{
    operator = ( channels );
}


Color::Color(const std::array<std::uint8_t, 4> &channels)
{
    operator = ( channels );
}


Color::Color( std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a )
{
    channels_[0] = r;
    channels_[1] = g;
    channels_[2] = b;
    channels_[3] = a;
}


std::array<std::uint8_t, 4> Color::channels() const
{
    return channels_;
}


/***
 * 3. Getters
 ***/

glm::vec3 Color::toVec3() const
{
    return glm::vec3(
                channels_[0] / 255.0f,
                channels_[1] / 255.0f,
                channels_[2] / 255.0f );
}


glm::vec4 Color::toVec4() const
{
    return glm::vec4(
                channels_[0] / 255.0f,
                channels_[1] / 255.0f,
                channels_[2] / 255.0f,
                channels_[3] / 255.0f
            );
}


/***
 * 4. Operators
 ***/

Color& Color::operator = ( const glm::vec3 &channels )
{
    unsigned int i = 0;

    for( ; i<3; i++ ){
        channels_[i] = static_cast< std::uint8_t >( channels[i] * 255.0f );
    }
    channels_[3] = 255;

    return *this;
}


Color& Color::operator = ( const glm::vec4 &channels )
{
    unsigned int i = 0;

    for( ; i<4; i++ ){
        channels_[i] = static_cast< std::uint8_t >( channels[i] * 255.0f );
    }

    return *this;
}


Color& Color::operator = ( const std::uint8_t *channels )
{
    unsigned int i = 0;

    for( ; i<4; i++ ){
        channels_[i] = channels[i];
    }

    return *this;
}


Color& Color::operator = ( const std::array<std::uint8_t, 4> &channels )
{
    unsigned int i = 0;

    for( ; i<4; i++ ){
        channels_[i] = channels[i];
    }

    return *this;
}


const std::uint8_t &Color::operator []( unsigned int index ) const
{
    return channels_[index];
}


std::uint8_t &Color::operator []( unsigned int index )
{
    return channels_[index];
}


bool Color::operator == ( const Color &b ) const
{
    return ( channels_ == b.channels_ );
}


bool Color::operator != ( const Color &b ) const
{
    return ( channels_ != b.channels_ );
}

} // namespace como
