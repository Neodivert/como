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
#include <cstdint>
#include <cstring> // memcpy

namespace como {

/***
 * 1. Construction
 ***/

PackableColor::PackableColor() :
    plainColor_( Color() )
{}


PackableColor::PackableColor( const Color &color ) :
    plainColor_( color )
{}


/***
 * 3. Getters
 ***/

Color PackableColor::getValue() const
{
    return plainColor_;
}


PacketSize PackableColor::getPacketSize() const
{
    return 4 * sizeof( std::uint8_t );
}


/***
 * 4. Setters
 ***/

void PackableColor::setValue( const Color& color )
{
    plainColor_ = color;
}


/***
 * 5. Packing / unpacking
 ***/

void *PackableColor::pack( void *buffer ) const
{
    std::uint8_t* castedBuffer = static_cast< std::uint8_t* >( buffer );

    memcpy( castedBuffer,
            plainColor_.channels().data(),
            plainColor_.channels().size() );

    castedBuffer += plainColor_.channels().size();

    return castedBuffer;
}


const void *PackableColor::unpack( const void *buffer )
{
    const std::uint8_t* castedBuffer = static_cast< const std::uint8_t* >( buffer );

    plainColor_ = castedBuffer;

    castedBuffer += plainColor_.channels().size();

    return castedBuffer;
}


const void *PackableColor::unpack( const void *buffer ) const
{
    const std::uint8_t* castedBuffer = static_cast< const std::uint8_t* >( buffer );

    const Color unpackedColor = castedBuffer;

    if( unpackedColor != plainColor_ ){
        throw std::runtime_error( "Unpacked unexpected color" );
    }

    castedBuffer += plainColor_.channels().size();

    return castedBuffer;
}

} // namespace como
