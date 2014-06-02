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

#include "packable_color.hpp"

namespace como {

/***
 * 1. Construction
 ***/


PackableColor::PackableColor()
{
    for( unsigned int i=0; i<4; i++ ){
        (*this)[i] = 0;
    }
}


PackableColor::PackableColor( const glm::vec3& channels )
{
    std::array< std::uint8_t, 4 > values = {{
                                                static_cast< std::uint8_t >( channels[0] * 255.0f ),
                                                static_cast< std::uint8_t >( channels[1] * 255.0f ),
                                                static_cast< std::uint8_t >( channels[2] * 255.0f ),
                                                255
                                            }};

    this->setValues( values );
}


PackableColor::PackableColor( const glm::vec4& channels )
{
    std::array< std::uint8_t, 4 > values = {{
                                                static_cast< std::uint8_t >( channels[0] * 255.0f ),
                                                static_cast< std::uint8_t >( channels[1] * 255.0f ),
                                                static_cast< std::uint8_t >( channels[2] * 255.0f ),
                                                static_cast< std::uint8_t >( channels[3] * 255.0f )
                                            }};

    this->setValues( values );
}


PackableColor::PackableColor( const std::uint8_t* channels ) :
    PackableArray( channels )
{}


PackableColor::PackableColor( const std::array< std::uint8_t, 4>& channels ) :
    PackableArray( &channels[0] )
{}


PackableColor::PackableColor( std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a )
{
    std::array< std::uint8_t, 4 > values = {{ r, g, b, a }};
    this->setValues( values );
}


/***
 * 3. Conversions
 ***/

glm::vec3 PackableColor::toVec3() const
{
    return glm::vec3(
                (*this)[0].getValue() / 255.0f,
                (*this)[1].getValue() / 255.0f,
                (*this)[2].getValue() / 255.0f
                );
}


glm::vec4 PackableColor::toVec4() const
{
    return glm::vec4(
                (*this)[0].getValue() / 255.0f,
                (*this)[1].getValue() / 255.0f,
                (*this)[2].getValue() / 255.0f,
                (*this)[3].getValue() / 255.0f
                );
}

} // namespace como
