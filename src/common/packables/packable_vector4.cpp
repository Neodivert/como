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

#include "packable_vector4.hpp"
#include <cstdio>
#include <locale>
#include <cstring>
#include <stdexcept>

namespace como {

const char FLOAT_TO_STRING_FORMAT[32] = "%f#%f#%f#%f";

/***
 * 1. Construction
 ***/

PackableVector4::PackableVector4()
{
    setValue( 0.0f, 0.0f, 0.0f, 0.0f );
}


PackableVector4::PackableVector4( float x, float y, float z, float w )
{
    setValue( x, y, z, w );
}


PackableVector4::PackableVector4( const float* values )
{
    setValue( values[0], values[1], values[2], values[3] );
}


PackableVector4::PackableVector4( const PackableVector4& b ) :
    Packable( b )
{
    setValue( b.values_[0], b.values_[1], b.values_[2], b.values_[3] );
}


PackableVector4::PackableVector4( PackableVector4&& b )
{
    setValue( b.values_[0], b.values_[1], b.values_[2], b.values_[3] );
}


/***
 * 3. Getters
 ***/

std::uint16_t PackableVector4::getPacketSize() const
{
    return PACK_STRING_SIZE;
}


const float* PackableVector4::getValue() const
{
    return values_;
}


const float& PackableVector4::operator []( unsigned int index ) const
{
    return values_[index];
}


/***
 * 4. Setters
 ***/

void PackableVector4::setValue( float x, float y, float z, float w )
{
    values_[0] = x;
    values_[1] = y;
    values_[2] = z;
    values_[3] = w;

    // Create the string representation of the transformation angle and vector.
    snprintf( packString_,
              PACK_STRING_SIZE,
              FLOAT_TO_STRING_FORMAT,
              values_[0],
              values_[1],
              values_[2],
              values_[3] );

    // The string representation of the float values may have a different
    // decimal separator between machines. Replace it with a dot for
    // network transfer.
    for( unsigned int i=0; i<PACK_STRING_SIZE; i++ ){
        if( packString_[i] == ( localeconv() )->decimal_point[0] ){
            packString_[i] = '.';
        }
    }
}


/***
 * 5. Packing and unpacking
 ***/

void* PackableVector4::pack( void* buffer ) const
{
    // Cast the buffer.
    char* castedBuffer = static_cast< char* >( buffer );

    // Pack this packable's content into the buffer.
    strncpy( castedBuffer, packString_, PACK_STRING_SIZE );

    // Return a pointer to the next position in buffer.
    return static_cast<void *>( castedBuffer + getPacketSize() );
}


const void* PackableVector4::unpack( const void* buffer )
{
    int sscanfReturnValue = 0;

    // Cast the buffer.
    const char* castedBuffer = static_cast<const char*>( buffer );

    // Unpack this packable's content from the buffer.
    strncpy( packString_, castedBuffer, PACK_STRING_SIZE );

    // The string representation of the float values may have a different
    // decimal separator between machines. Replace the dots (used for network
    // transfer) with the local decimal separator.
    for( unsigned int i=0; i<PACK_STRING_SIZE; i++ ){
        if( packString_[i] == '.' ){
            packString_[i] = ( localeconv() )->decimal_point[0];
        }
    }

    // Translate the previous transformation magnitude from string format to
    // float one.
    sscanfReturnValue = sscanf( packString_,
                                FLOAT_TO_STRING_FORMAT,
                                &( values_[0] ),
                                &( values_[1] ),
                                &( values_[2] ),
                                &( values_[3] )
                                );

    if( ( sscanfReturnValue < 4 ) || ( sscanfReturnValue == EOF ) ){
        throw std::runtime_error( std::string( "ERROR when unpacking vector4 with sscanf: return value(" ) +
                                  std::to_string( sscanfReturnValue ) +
                                  std::string( ")" ) );
    }

    // Return a pointer to the next position in buffer.
    return static_cast<const void *>( castedBuffer + getPacketSize() );
}


const void* PackableVector4::unpack( const void* buffer ) const
{
    float unpackedValues[4];
    char unpackedString[PACK_STRING_SIZE];
    unsigned int i;

    int sscanfReturnValue = 0;

    // Cast the buffer.
    const char* castedBuffer = static_cast<const char*>( buffer );

    // Unpack this packable's content from the buffer.
    strncpy( unpackedString, castedBuffer, PACK_STRING_SIZE );

    // The string representation of the float values may have a different
    // decimal separator between machines. Replace the dots (used for network
    // transfer) with the local decimal separator.
    for( i=0; i<PACK_STRING_SIZE; i++ ){
        if( unpackedString[i] == '.' ){
            unpackedString[i] = ( localeconv() )->decimal_point[0];
        }
    }

    // Translate the previous transformation magnitude from string format to
    // float one.
    sscanfReturnValue = sscanf( unpackedString,
                                FLOAT_TO_STRING_FORMAT,
                                &( unpackedValues[0] ),
                                &( unpackedValues[1] ),
                                &( unpackedValues[2] ),
                                &( unpackedValues[3] )
                                );

    if( ( sscanfReturnValue < 4 ) || ( sscanfReturnValue == EOF ) ){
        throw std::runtime_error( std::string( "ERROR when unpacking vector4 with sscanf: return value(" ) +
                                  std::to_string( sscanfReturnValue ) +
                                  std::string( ")" ) );
    }

    for( i=0; i<4; i++ ){
        if( unpackedValues[i] != values_[i] ){
            throw std::runtime_error( "ERROR: Unpacked an unexpected PackableVector4" );
        }
    }

    // Return a pointer to the next position in buffer.
    return static_cast<const void *>( castedBuffer + getPacketSize() );
}


/***
 * 6. Operators
 ***/

PackableVector4& PackableVector4::operator = ( const PackableVector4& b )
{
    if( this != &b ){
        setValue( b.values_[0], b.values_[1], b.values_[2], b.values_[3] );
    }

    return *this;
}


PackableVector4& PackableVector4::operator = ( const float* values )
{
    setValue( values[0], values[1], values[2], values[3] );

    return *this;
}

} // namespace como
