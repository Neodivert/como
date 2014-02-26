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
#include <array>

namespace como {

template <class ElementType, unsigned int ARRAY_SIZE>
class PackableArray : public Packable
{
    private:
        ElementType array_[ARRAY_SIZE];

    public:
        /***
         * 1. Construction
         ***/
        PackableArray() = default;
        PackableArray( const PackableArray& b );
        PackableArray( const ElementType* str );
        PackableArray( PackableArray&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~PackableArray(){}


        /***
         * 3. Packing and unpacking
         ***/
        virtual void* pack( void* buffer ) const ;
        virtual const void* unpack( const void* buffer ) ;


        /***
         * 4. Getters
         ***/
        const ElementType* getValue() const { return array_; }
        virtual std::uint16_t getPacketSize() const { return ARRAY_SIZE; }
        ElementType& operator []( unsigned int index ){ return array_[index]; }
        const ElementType& operator []( unsigned int index ) const { return array_[index]; }

        /***
         * 5. Setters
         ***/
        void setValue( const ElementType* str ){ memcpy( array_, str, ARRAY_SIZE ); }


        /***
         * 6. Operators
         ***/
        PackableArray& operator = (const PackableArray& b);
        PackableArray& operator = ( const ElementType* array );
        PackableArray& operator = ( PackableArray&& ) = default;
};


/***
 * 1. Construction
 ***/

template <class ElementType, unsigned int ARRAY_SIZE>
PackableArray<ElementType, ARRAY_SIZE>::PackableArray( const PackableArray<ElementType, ARRAY_SIZE>& b ) :
    Packable()
{
    memcpy( array_, b.array_, ARRAY_SIZE );
}

template <class ElementType, unsigned int ARRAY_SIZE>
PackableArray<ElementType, ARRAY_SIZE>::PackableArray( const ElementType* str ) :
    Packable()
{
    memcpy( array_, str, ARRAY_SIZE );
}


/***
 * 3. Packing and unpacking
 ***/

template <class ElementType, unsigned int ARRAY_SIZE>
void* PackableArray<ElementType, ARRAY_SIZE>::pack( void* buffer ) const
{
    ElementType* castedBuffer = static_cast< ElementType* >( buffer );

    memcpy( castedBuffer, array_, ARRAY_SIZE );

    return static_cast< void *>( castedBuffer + ARRAY_SIZE );
}


template <class ElementType, unsigned int ARRAY_SIZE>
const void* PackableArray<ElementType, ARRAY_SIZE>::unpack( const void* buffer )
{
    const ElementType* castedBuffer = static_cast< const ElementType* >( buffer );

    memcpy( array_, static_cast< const ElementType* >( buffer ), ARRAY_SIZE );

    return static_cast< const void *>( castedBuffer + ARRAY_SIZE );
}


/***
 * 6. Operators
 ***/

template <class ElementType, unsigned int ARRAY_SIZE>
PackableArray<ElementType, ARRAY_SIZE>& PackableArray<ElementType, ARRAY_SIZE>::operator = ( const PackableArray<ElementType, ARRAY_SIZE>& b )
{
    if( this != &b ){
        memcpy( array_, b.array_, ARRAY_SIZE );
    }
    return *this;
}

template <class ElementType, unsigned int ARRAY_SIZE>
PackableArray<ElementType, ARRAY_SIZE>& PackableArray<ElementType, ARRAY_SIZE>::operator = ( const ElementType* str )
{
    memcpy( array_, str, ARRAY_SIZE );

    return *this;
}


} // namespace como

#endif // PACKABLE_STRING_HPP
