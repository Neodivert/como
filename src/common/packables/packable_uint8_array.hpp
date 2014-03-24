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

#ifndef PACKABLE_UINT8_ARRAY_HPP
#define PACKABLE_UINT8_ARRAY_HPP

#include "packable_array.hpp"

namespace como {

template <class UnpackedType, unsigned int ARRAY_SIZE>
class PackableUint8Array : public PackableArray<UnpackedType, ARRAY_SIZE>
{
    public:
        /***
         * 1. Construction
         ***/
        PackableUint8Array() = default;
        PackableUint8Array( const UnpackedType* values ) : PackableArray<UnpackedType, ARRAY_SIZE>( values ){}
        PackableUint8Array( const PackableUint8Array& ) = default;
        PackableUint8Array( PackableUint8Array&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~PackableUint8Array() = default;


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const { return sizeof( std::uint8_t ) * ARRAY_SIZE; }


        /***
         * 4. Packing and unpacking
         ***/
        virtual void* pack( void* buffer ) const;
        virtual const void* unpack( const void* buffer );
        virtual const void* unpack( const void* buffer ) const;


        /***
         * 6. Operators
         ***/
        PackableUint8Array<UnpackedType, ARRAY_SIZE>& operator = ( const PackableUint8Array<UnpackedType, ARRAY_SIZE>& ) = default;
        //PackableUint8Array<UnpackedType, ARRAY_SIZE>& operator = ( const UnpackedType& value );
        PackableUint8Array<UnpackedType, ARRAY_SIZE>& operator = ( PackableUint8Array<UnpackedType, ARRAY_SIZE>&& ) = default;
};

typedef PackableUint8Array<std::uint8_t, 4> PackableRGBA;


/***
 * 5. Packing and unpacking
 ***/

template <class UnpackedType, unsigned int ARRAY_SIZE>
void* PackableUint8Array<UnpackedType, ARRAY_SIZE>::pack( void* buffer ) const
{
    unsigned int i;

    // Cast the buffer.
    std::uint8_t* castedBuffer = static_cast< std::uint8_t* >( buffer );

    // Pack the wrapper's inner values into the buffer.
    for( i=0; i<ARRAY_SIZE; i++ ){
        castedBuffer[i] = static_cast< std::uint8_t >( this->values_[i] );
    }

    // Return a pointer to the next position in buffer.
    return static_cast< void* >( castedBuffer + ARRAY_SIZE );
}


template <class UnpackedType, unsigned int ARRAY_SIZE>
const void* PackableUint8Array<UnpackedType, ARRAY_SIZE>::unpack( const void* buffer )
{
    unsigned int i;

    // Cast buffer to the UnpackedType type.
    const UnpackedType* castedBuffer = static_cast< const UnpackedType* >( buffer );

    // Unpack the wrapper's inner values from the buffer.
    for( i=0; i<ARRAY_SIZE; i++ ){
        this->values_[i] = static_cast< UnpackedType >( castedBuffer[i] );
    }

    // Return a pointer to the next position in buffer.
    return static_cast< const void* >( castedBuffer + ARRAY_SIZE );
}


template <class UnpackedType, unsigned int ARRAY_SIZE>
const void* PackableUint8Array<UnpackedType, ARRAY_SIZE>::unpack( const void* buffer ) const
{
    UnpackedType unpackedValue;

    unsigned int i;

    // Cast buffer to the UnpackedType type.
    const UnpackedType* castedBuffer = static_cast< const UnpackedType* >( buffer );

    // Unpack the wrapper's inner values from the buffer.
    for( i=0; i<ARRAY_SIZE; i++ ){
        unpackedValue = static_cast< UnpackedType >( castedBuffer[i] );

        if( unpackedValue != this->values_[i] ){
            throw std::runtime_error( "ERROR: Unpacked an unexpected PackableUint8Array" );
        }
    }

    // Return a pointer to the next position in buffer.
    return static_cast< const void* >( castedBuffer + ARRAY_SIZE );
}

} // namespace como

#endif // PACKABLE_UINT8_ARRAY_HPP
