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

#include "packable_string.hpp"
#include "packable_integer.hpp"
#include <cstring>

namespace como {

/***
 * 1. Construction
 ***/

PackableString::PackableString( const PackableString& b ) :
    Packable(),
    str_( b.str_ )
{}

PackableString::PackableString( const char* str ) :
    Packable(),
    str_( str )
{}

PackableString::PackableString( const std::string& str ) :
    Packable(),
    str_( str )
{}


/***
 * 3. Packing and unpacking
 ***/

void* PackableString::pack( void* buffer ) const
{
    const PackableUint16< size_t > strSize( str_.size() );

    // Pack the std::string size into the buffer.
    buffer = strSize.pack( buffer );

    // Cast the buffer so we can pack this std::string.
    char* castedBuffer = static_cast< char* >( buffer );

    // Pack the std::string into the buffer.
    strncpy( castedBuffer, str_.substr( 0, strSize.getValue() ).c_str(), strSize.getValue() );

    // Return a pointer to the next position in the buffer.
    return static_cast< void *>( castedBuffer + strSize.getValue() );
}


const void* PackableString::unpack( const void* buffer )
{
    PackableUint16< size_t > strSize;

    buffer = strSize.unpack( buffer );

    // Cast the buffer so we can unpack the std::string.
    const char* castedBuffer = static_cast< const char* >( buffer );

    // Unpack the std::string from the buffer.
    str_ = std::string( castedBuffer, castedBuffer + strSize.getValue() );

    // Return a pointer to the next position in the buffer.
    return static_cast< const void *>( castedBuffer + strSize.getValue() );
}


const void* PackableString::unpack( const void* buffer ) const
{
    PackableUint16< size_t > strSize( str_.size() );
    std::string unpackedStr;

    buffer = strSize.unpack( buffer );

    // Cast the buffer so we can unpack the std::string.
    const char* castedBuffer = static_cast< const char* >( buffer );

    // Unpack a std::string from the given buffer.
    unpackedStr = std::string( castedBuffer, castedBuffer + strSize.getValue() );

    // Throw an exception if the unpacked std::string doesn't match str_.
    if( str_ != unpackedStr ){
        throw std::runtime_error( "ERROR: Unpacked an unexpected PackableString" );
    }

    // Return a pointer to the next position in the buffer.
    return static_cast< const void *>( castedBuffer + strSize.getValue() );
}


/***
 * 6. Operators
 ***/

PackableString& PackableString::operator = ( const PackableString& b )
{
    if( this != &b ){
        str_ = b.str_;
    }
    return *this;
}

PackableString& PackableString::operator = ( const char* str )
{
    str_ = str;

    return *this;
}

PackableString& PackableString::operator = ( const std::string& str )
{
    str_ = str;

    return *this;
}

} // namespace como
