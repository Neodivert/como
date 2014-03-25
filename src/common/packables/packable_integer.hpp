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

#ifndef PACKABLE_INTEGER_HPP
#define PACKABLE_INTEGER_HPP

#include "packable.hpp"
#include <cstdint>

namespace como {

template <class PackedType, class UnpackedType>
class PackableInteger : public Packable {
    protected:
        UnpackedType value_;

    public:
        /***
         * 1. Construction
         ***/
        PackableInteger() = default;
        PackableInteger( const UnpackedType& value ) : value_( value ){}
        PackableInteger( const PackableInteger& ) = default;
        PackableInteger( PackableInteger&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~PackableInteger() = default;


        /***
         * 3. Getters
         ***/
        UnpackedType getValue() const { return this->value_; }
        virtual std::uint16_t getPacketSize() const = 0;


        /***
         * 4. Setters
         ***/
        void setValue( UnpackedType value ){ this->value_ = value; }


        /***
         * 5. Packing and unpacking
         ***/
        virtual void* pack( void* buffer ) const;
        virtual const void* unpack( const void* buffer );
        virtual const void* unpack( const void* buffer ) const;


        /***
         * 6. Auxiliar methods
         ***/
        virtual PackedType flipByteOrder( const PackedType& ) const = 0;


        /***
         * 7. Operators
         ***/
        PackableInteger<PackedType, UnpackedType>& operator = ( const PackableInteger<PackedType, UnpackedType>& b );
        PackableInteger<PackedType, UnpackedType>& operator = ( const UnpackedType& value );
        PackableInteger<PackedType, UnpackedType>& operator = ( PackableInteger<PackedType, UnpackedType>&& b );
};


/***
 * 5. Packing and unpacking
 ***/

template <class PackedType, class UnpackedType>
void* PackableInteger<PackedType, UnpackedType>::pack( void* buffer ) const
{
    PackedType networkValue;

    // Cast the buffer.
    PackedType* castedBuffer = static_cast< PackedType* >( buffer );

    // Get the wrapper's inner value.
    networkValue = static_cast< PackedType >( this->value_ );

    // If necessary, translate value to network order.
#if LITTLE_ENDIAN
    networkValue = flipByteOrder( networkValue );
#endif

    // Pack the wrapper's inner value into the buffer.
    *castedBuffer = networkValue;

    // Return a pointer to the next position in buffer.
    return static_cast< void* >( castedBuffer + 1 );
}


template <class PackedType, class UnpackedType>
const void* PackableInteger<PackedType, UnpackedType>::unpack( const void* buffer )
{
    // Cast buffer to the PackedType type.
    const PackedType* castedBuffer = static_cast< const PackedType* >( buffer );

    // Unpack the wrapper's inner value from the buffer.
    PackedType networkValue = static_cast< PackedType >( *castedBuffer );

    // If necessary, translate value from network order.
#if LITTLE_ENDIAN
    networkValue = flipByteOrder( networkValue );
#endif

    // Save unpacked vlaue into this instance.
    this->value_ = static_cast< UnpackedType >( networkValue );

    // Return a pointer to the next position in buffer.
    return static_cast< const void* >( castedBuffer + 1 );
}


template <class PackedType, class UnpackedType>
const void* PackableInteger<PackedType, UnpackedType>::unpack( const void* buffer ) const
{
    PackedType networkValue;

    // Cast buffer to the UnpackedType type.
    const PackedType* castedBuffer = static_cast< const PackedType* >( buffer );

    // Unpack the wrapper's inner value from the buffer.
    networkValue = static_cast< PackedType >( *castedBuffer );

    // If necessary, translate value from network order.
#if LITTLE_ENDIAN
    networkValue = flipByteOrder( networkValue );
#endif

    // If the unpacked value isn't the expected one, throw an exception.
    if( static_cast< UnpackedType >( networkValue ) != value_ ){
        throw std::runtime_error( "ERROR: Unpacked an unexpected PackableUint16" );
    }

    // Return a pointer to the next position in buffer.
    return static_cast< const void* >( castedBuffer + 1 );
}


/***
 * 7. Operators
 ***/

template <class PackedType, class UnpackedType>
PackableInteger<PackedType, UnpackedType>& PackableInteger<PackedType, UnpackedType>::operator = ( const PackableInteger<PackedType, UnpackedType>& b )
{
    if( this != &b ){
        value_ = b.value_;
    }
    return *this;
}


template <class PackedType, class UnpackedType>
PackableInteger<PackedType, UnpackedType>& PackableInteger<PackedType, UnpackedType>::operator = ( const UnpackedType& value )
{
    value_ = value;

    return *this;
}


template <class PackedType, class UnpackedType>
PackableInteger<PackedType, UnpackedType>& PackableInteger<PackedType, UnpackedType>::operator = ( PackableInteger<PackedType, UnpackedType>&& b )
{
    if( this != &b ){
        value_ = b.value_;
    }
    return *this;
}

} // namespace como

#endif // PACKABLE_INTEGER_HPP
