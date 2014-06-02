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

#include "packable_float.hpp"
#include <common/packables/packable_uint32.hpp>
#include <cmath>

namespace como {

const float FLOAT_PRECISION = 10000.0f;


/***
 * 1. Construction
 ***/

PackableFloat::PackableFloat( float value )
 : PackableWrapper< float >( value )
{}


/***
 * 3. Packing and unpacking
 ***/

void* PackableFloat::pack( void* buffer ) const
{
    PackableUint32< std::int32_t > integralPart;
    PackableUint32< std::int32_t > fractionalPart;

    // Get the value's integral and fractional parts.
    integralPart = static_cast< std::int32_t >( getValue() );
    fractionalPart = ( getValue() - integralPart.getValue() ) * FLOAT_PRECISION;

    // Pack previous values.
    buffer = integralPart.pack( buffer );
    buffer = fractionalPart.pack( buffer );

    return buffer;
}

const void* PackableFloat::unpack( const void* buffer )
{
    PackableUint32< std::int32_t > integralPart;
    PackableUint32< std::int32_t > fractionalPart;

    // Unpack the value's integral and fractional parts from buffer and
    // build the float from them.
    buffer = integralPart.unpack( buffer );
    buffer = fractionalPart.unpack( buffer );

    setValue( static_cast< float >( integralPart.getValue() ) + static_cast< float >( fractionalPart.getValue() / FLOAT_PRECISION ) );

    return buffer;
}


const void* PackableFloat::unpack( const void* buffer ) const
{
    // Get the current value's integral and fractional parts.
    const PackableUint32< std::int32_t > integralPart( static_cast< std::int32_t >( getValue() ) );
    const PackableUint32< std::int32_t > fractionalPart( ( getValue() - integralPart.getValue() ) * FLOAT_PRECISION );

    // Unpack the integral and fractional parts from buffer and check if they
    // match the previous ones.
    buffer = integralPart.unpack( buffer );
    buffer = fractionalPart.unpack( buffer );

    return buffer;
}


/***
 * 4. Getters
 ***/

PacketSize PackableFloat::getPacketSize() const
{
    return 8;   // Two 4 bytes integers for value's both integral and
                // fractional parts.
}

} // namespace como
