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

#ifndef PACKABLE_ARRAY_WRAPPER_HPP
#define PACKABLE_ARRAY_WRAPPER_HPP

#include "packable.hpp"
#include <cstdint>

namespace como {

template <class UnpackedType, unsigned int ARRAY_SIZE>
class PackableArrayWrapper : public Packable {
    protected:
        UnpackedType values_[ARRAY_SIZE];

    public:
        /***
         * 1. Construction
         ***/
        PackableArrayWrapper<UnpackedType, ARRAY_SIZE>() = default;
        PackableArrayWrapper<UnpackedType, ARRAY_SIZE>( const UnpackedType* values );
        PackableArrayWrapper<UnpackedType, ARRAY_SIZE>( const PackableArrayWrapper<UnpackedType, ARRAY_SIZE>& ) = default;
        PackableArrayWrapper<UnpackedType, ARRAY_SIZE>( PackableArrayWrapper<UnpackedType, ARRAY_SIZE>&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~PackableArrayWrapper() = default;


        /***
         * 3. Getters
         ***/
        const UnpackedType* getValue() const ;
        virtual std::uint16_t getPacketSize() const = 0;
        UnpackedType& operator []( unsigned int index );
        const UnpackedType& operator []( unsigned int index ) const;


        /***
         * 4. Setters
         ***/
        void setValue( UnpackedType* values );


        /***
         * 5. Packing and unpacking
         ***/
        virtual void* pack( void* buffer ) const = 0;
        virtual const void* unpack( const void* buffer ) = 0;


        /***
         * 6. Operators
         ***/
        PackableArrayWrapper<UnpackedType, ARRAY_SIZE>& operator = ( const PackableArrayWrapper<UnpackedType, ARRAY_SIZE>& b );
        PackableArrayWrapper<UnpackedType, ARRAY_SIZE>& operator = ( const UnpackedType* value );
        PackableArrayWrapper<UnpackedType, ARRAY_SIZE>& operator = ( PackableArrayWrapper<UnpackedType, ARRAY_SIZE>&& ) = delete;
};


/***
 * 1. Construction
 ***/

template <class UnpackedType, unsigned int ARRAY_SIZE>
PackableArrayWrapper<UnpackedType, ARRAY_SIZE>::PackableArrayWrapper( const UnpackedType* values )
{
    unsigned int i;

    for( i=0; i<ARRAY_SIZE; i++ ){
        values_[i] = values[i];
    }
}


/***
 * 3. Getters
 ***/

template <class UnpackedType, unsigned int ARRAY_SIZE>
const UnpackedType* PackableArrayWrapper<UnpackedType, ARRAY_SIZE>::getValue() const
{
    return values_;
}


template <class UnpackedType, unsigned int ARRAY_SIZE>
UnpackedType& PackableArrayWrapper<UnpackedType, ARRAY_SIZE>::operator []( unsigned int index )
{
    return values_[index];
}


template <class UnpackedType, unsigned int ARRAY_SIZE>
const UnpackedType& PackableArrayWrapper<UnpackedType, ARRAY_SIZE>::operator []( unsigned int index ) const
{
    return values_[index];
}


/***
 * 4. Setters
 ***/

template <class UnpackedType, unsigned int ARRAY_SIZE>
void PackableArrayWrapper<UnpackedType, ARRAY_SIZE>::setValue( UnpackedType* values )
{
    unsigned int i;

    for( i=0; i<ARRAY_SIZE; i++ ){
        values_[i] = values[i];
    }
}


/***
 * 6. Operators
 ***/

template <class UnpackedType, unsigned int ARRAY_SIZE>
PackableArrayWrapper<UnpackedType, ARRAY_SIZE>& PackableArrayWrapper<UnpackedType, ARRAY_SIZE>::operator = ( const PackableArrayWrapper<UnpackedType, ARRAY_SIZE>& b )
{
    unsigned int i;

    if( this != &b ){
        for( i=0; i<ARRAY_SIZE; i++ ){
            values_[i] = b.values_[i];
        }
    }

    return *this;
}


template <class UnpackedType, unsigned int ARRAY_SIZE>
PackableArrayWrapper<UnpackedType, ARRAY_SIZE>& PackableArrayWrapper<UnpackedType, ARRAY_SIZE>::operator = ( const UnpackedType* values )
{
    unsigned int i;

    for( i=0; i<ARRAY_SIZE; i++ ){
        values_[i] = values[i];
    }

    return *this;
}

} // namespace como

#endif // PACKABLE_ARRAY_WRAPPER_HPP
