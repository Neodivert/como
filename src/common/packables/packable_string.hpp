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

#ifndef PACKABLE_STRING_HPP
#define PACKABLE_STRING_HPP

#include "packable.hpp"
#include <cstring>

namespace como {

template <unsigned int STRING_SIZE>
class PackableString : public Packable
{
    private:
        char str_[STRING_SIZE];

    public:
        /***
         * 1. Construction
         ***/
        PackableString() = default;
        PackableString( const PackableString& b );
        PackableString( const char* str );
        PackableString( PackableString&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~PackableString(){}


        /***
         * 3. Packing and unpacking
         ***/
        virtual void* pack( void* buffer ) const ;
        virtual const void* unpack( const void* buffer ) ;
        virtual const void* unpack( const void* buffer ) const;


        /***
         * 4. Getters
         ***/
        const char* getValue() const { return str_; }
        virtual std::uint16_t getPacketSize() const { return STRING_SIZE; }
        char& operator []( unsigned int index ){ return str_[index]; }
        const char& operator []( unsigned int index ) const { return str_[index]; }

        /***
         * 5. Setters
         ***/
        void setValue( const char* str ){ strncpy( str_, str, STRING_SIZE ); }


        /***
         * 6. Operators
         ***/
        PackableString& operator = (const PackableString& b);
        PackableString& operator = ( const char* str );
        PackableString& operator = ( PackableString&& ) = default;
};


/***
 * 1. Construction
 ***/

template <unsigned int STRING_SIZE>
PackableString<STRING_SIZE>::PackableString( const PackableString<STRING_SIZE>& b ) :
    Packable()
{
    strncpy( str_, b.str_, STRING_SIZE );
}

template <unsigned int STRING_SIZE>
PackableString<STRING_SIZE>::PackableString( const char* str ) :
    Packable()
{
    strncpy( str_, str, STRING_SIZE );
}


/***
 * 3. Packing and unpacking
 ***/

template <unsigned int STRING_SIZE>
void* PackableString<STRING_SIZE>::pack( void* buffer ) const
{
    char* castedBuffer = static_cast< char* >( buffer );

    strncpy( castedBuffer, str_, STRING_SIZE );

    return static_cast< void *>( castedBuffer + STRING_SIZE );
}


template <unsigned int STRING_SIZE>
const void* PackableString<STRING_SIZE>::unpack( const void* buffer )
{
    const char* castedBuffer = static_cast< const char* >( buffer );

    strncpy( str_, static_cast< const char* >( buffer ), STRING_SIZE );

    return static_cast< const void *>( castedBuffer + STRING_SIZE );
}


template <unsigned int STRING_SIZE>
const void* PackableString<STRING_SIZE>::unpack( const void* buffer ) const
{
    char unpackedStr[STRING_SIZE] = { 0 };

    const char* castedBuffer = static_cast< const char* >( buffer );

    strncpy( unpackedStr, static_cast< const char* >( buffer ), STRING_SIZE );

    if( strncmp( str_, unpackedStr, STRING_SIZE ) ){
        throw std::runtime_error( "ERROR: Unpacked an unexpected PackableString" );
    }

    return static_cast< const void *>( castedBuffer + STRING_SIZE );
}


/***
 * 6. Operators
 ***/

template <unsigned int STRING_SIZE>
PackableString<STRING_SIZE>& PackableString<STRING_SIZE>::operator = ( const PackableString<STRING_SIZE>& b )
{
    if( this != &b ){
        strncpy( str_, b.str_, STRING_SIZE );
    }
    return *this;
}

template <unsigned int STRING_SIZE>
PackableString<STRING_SIZE>& PackableString<STRING_SIZE>::operator = ( const char* str )
{
    strncpy( str_, str, STRING_SIZE );

    return *this;
}


} // namespace como

#endif // PACKABLE_STRING_HPP
