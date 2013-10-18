/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
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

#include "endianness.hpp"

namespace como {

#define LITTLE_ENDIAN 1


/***
 * Byte order flipping
 ***/

std::uint16_t flipByteOrder( const std::uint16_t& value )
{
    return ( (value & 0xFF00) >> 8 ) | ((value & 0x00FF ) << 8);
}


std::uint32_t flipByteOrder( const std::uint32_t& value )
{
    return ( (value & 0xFF000000) >> 24 ) |
           ( (value & 0x00FF0000) >> 8 ) |
           ( (value & 0x0000FF00) << 8 ) |
           ( (value & 0x000000FF) << 24 );
}


/***
 * 1. Uint16 translation
 ***/


std::uint16_t translateToNetworkOrder( const std::uint16_t& value )
{
#if LITTLE_ENDIAN
    return flipByteOrder( value );
#else
    return value;
#endif
}


std::uint16_t translateFromNetworkOrder( const std::uint16_t& value )
{
#if LITTLE_ENDIAN
    return flipByteOrder( value );
#else
    return value;
#endif
}


/***
 * 2. Uint32 translation
 ***/


std::uint32_t translateToNetworkOrder( const std::uint32_t& value )
{
#if LITTLE_ENDIAN
    return flipByteOrder( value );
#else
    return value;
#endif
}


std::uint32_t translateFromNetworkOrder( const std::uint32_t& value )
{
#if LITTLE_ENDIAN
    return flipByteOrder( value );
#else
    return value;
#endif
}

} // namespace como
