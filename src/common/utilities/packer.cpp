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

#include "packer.hpp"
#include <cstring>

namespace como {

namespace packer {

#define LITTLE_ENDIAN 1


/***
 * 1. Packing
 ***/

void pack( const std::uint8_t& value, char*& buffer )
{
    *( reinterpret_cast< std::uint8_t* >( buffer ) ) = value;

    buffer++;
}


void pack( const std::uint16_t& value, char*& buffer )
{
    *( reinterpret_cast< std::uint16_t* >( buffer ) ) = translateToNetworkOrder( value );

    buffer += 2;
}


void pack( const std::uint32_t& value, char*& buffer )
{
    *( reinterpret_cast< std::uint32_t* >( buffer ) ) = translateToNetworkOrder( value );

    buffer += 4;
}


void pack( const char* str, char*& buffer, const unsigned int n )
{
    memcpy( buffer, str, n );

    buffer += n;
}



/***
 * 2. Unpacking
 ***/

void unpack( std::uint8_t& value, const char*& buffer )
{
    value = *( reinterpret_cast< const std::uint8_t* >( buffer ) );

    buffer++;
}


void unpack( std::uint16_t& value, const char*& buffer )
{
    value = translateFromNetworkOrder( *( reinterpret_cast< const std::uint16_t* >( buffer ) ) ) ;

    buffer += 2;
}


void unpack( std::uint32_t& value, const char*& buffer )
{
    value = translateFromNetworkOrder( *( reinterpret_cast< const std::uint32_t* >( buffer ) ) );

    buffer += 4;
}


void unpack( char* str, const char*& buffer, const unsigned int n )
{
    memcpy( str, buffer, n );
}


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
 * 2. Uint16 translation
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
 * 3. Uint32 translation
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

} // namespace packer

} // namespace como
