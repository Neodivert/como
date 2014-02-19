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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "packable_wrapper_uint8.hpp"

namespace como {

/***
 * 1. Construction
 ***/

PackableWrapperUint8::PackableWrapperUint8( std::uint8_t* value ) :
    PackableWrapper<std::uint8_t>( value )
{}


/***
 * 3. Packing and unpacking
 ***/

void* PackableWrapperUint8::pack( void* buffer ) const
{
    // Pack the wrapped value in the buffer.
    *( static_cast< std::uint8_t* >( buffer ) ) = *value_;

    // Return a pointer to the next position in the buffer.
    return static_cast< std::uint8_t* >( buffer ) + 1;
}

const void* PackableWrapperUint8::unpack( const void* buffer )
{
    // Unpack the wrapped value from the buffer.
    *value_ = *( static_cast< const std::uint8_t* >( buffer ) );

    // Return a pointer to the next position in the buffer.
    return static_cast< const std::uint8_t* >( buffer ) + 1;
}


/***
 * 4. Getters
 ***/

std::uint16_t PackableWrapperUint8::getPacketSize() const
{
    return sizeof( std::uint8_t );
}


} // namespace como
