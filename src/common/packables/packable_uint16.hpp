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

#ifndef PACKABLE_UINT16_HPP
#define PACKABLE_UINT16_HPP

#include "packable_wrapper.hpp"

namespace como {

template <class UnpackedType>
class PackableUint16 : public PackableWrapper<UnpackedType>
{
    using PackableWrapper<UnpackedType>::value_;

    public:
        /***
         * 1. Construction
         ***/
        PackableUint16() = default;
        PackableUint16( const UnpackedType& value ) : PackableWrapper<UnpackedType>( value ){}
        PackableUint16( const PackableUint16& ) = default;
        PackableUint16( PackableUint16&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~PackableUint16() = default;


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const { return sizeof( std::uint16_t ); }


        /***
         * 4. Packing and unpacking
         ***/
        virtual void* pack( void* buffer ) const;
        virtual const void* unpack( const void* buffer );
        virtual const void* unpack( const void* buffer ) const;


        /***
         * 5. Operators
         ***/
        PackableUint16<UnpackedType>& operator = ( const PackableUint16<UnpackedType>& b );
        PackableUint16<UnpackedType>& operator = ( const UnpackedType& value );
        PackableUint16<UnpackedType>& operator = ( PackableUint16<UnpackedType>&& ) = delete;
};


/***
 * 5. Packing and unpacking
 ***/

template <class UnpackedType >
void* PackableUint16<UnpackedType>::pack( void* buffer ) const
{
    std::uint16_t networkValue;

    // Cast the buffer.
    std::uint16_t* castedBuffer = static_cast< std::uint16_t* >( buffer );

    // Get the wrapper's inner value and translate it to network order.
    networkValue = static_cast< std::uint16_t >( this->value_ );
#if LITTLE_ENDIAN
    networkValue = ( (networkValue & 0xFF00) >> 8 ) | ((networkValue & 0x00FF ) << 8);
#endif

    // Pack the wrapper's inner value into the buffer.
    *castedBuffer = networkValue;

    // Return a pointer to the next position in buffer.
    return static_cast< void* >( castedBuffer + 1 );
}


template <class UnpackedType>
const void* PackableUint16<UnpackedType>::unpack( const void* buffer )
{
    // Cast buffer to the UnpackedType type.
    const UnpackedType* castedBuffer = static_cast< const UnpackedType* >( buffer );

    // Unpack the wrapper's inner valued from the buffer and translate it from network order.
    this->value_ = static_cast< UnpackedType >( *castedBuffer );
#if LITTLE_ENDIAN
    value_ = ( (value_ & 0xFF00) >> 8 ) | ((value_ & 0x00FF ) << 8);
#endif

    // Return a pointer to the next position in buffer.
    return static_cast< const void* >( castedBuffer + 1 );
}


template <class UnpackedType>
const void* PackableUint16<UnpackedType>::unpack( const void* buffer ) const
{
    UnpackedType unpackedValue;

    // Cast buffer to the UnpackedType type.
    const UnpackedType* castedBuffer = static_cast< const UnpackedType* >( buffer );

    // Unpack the wrapper's inner valued from the buffer and translate it from network order.
    unpackedValue = static_cast< UnpackedType >( *castedBuffer );
#if LITTLE_ENDIAN
    unpackedValue = ( (unpackedValue & 0xFF00) >> 8 ) | ((unpackedValue & 0x00FF ) << 8);
#endif

    if( unpackedValue != value_ ){
        throw std::runtime_error( "ERROR: Unpacked an unexpected PackableUint16" );
    }

    // Return a pointer to the next position in buffer.
    return static_cast< const void* >( castedBuffer + 1 );
}


/***
 * 5. Operators
 ***/

template <class UnpackedType>
PackableUint16<UnpackedType>& PackableUint16<UnpackedType>::operator = ( const PackableUint16<UnpackedType>& b )
{
    PackableWrapper<UnpackedType>::operator =( b );

    return *this;
}


template <class UnpackedType>
PackableUint16<UnpackedType>& PackableUint16<UnpackedType>::operator = ( const UnpackedType& value )
{
    PackableWrapper<UnpackedType>::operator =( value );

    return *this;
}



} // namespace como

#endif // PACKABLE_UINT16_HPP
