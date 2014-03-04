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

#ifndef PACKABLE_UINT32_HPP
#define PACKABLE_UINT32_HPP

#include "packable_wrapper.hpp"

namespace como {

template <class UnpackedType>
class PackableUint32 : public PackableWrapper<UnpackedType>
{
    public:
        /***
         * 1. Construction
         ***/
        PackableUint32() = default;
        PackableUint32( const UnpackedType& value ) : PackableWrapper<UnpackedType>( value ){}
        PackableUint32( const PackableUint32& ) = default;
        PackableUint32( PackableUint32&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~PackableUint32() = default;


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const { return sizeof( std::uint32_t ); }


        /***
         * 4. Packing and unpacking
         ***/
        virtual void* pack( void* buffer ) const ;
        virtual const void* unpack( const void* buffer ) ;


        /***
         * 5. Operators
         ***/
        PackableUint32<UnpackedType>& operator = ( const PackableUint32<UnpackedType>& b );
        PackableUint32<UnpackedType>& operator = ( const UnpackedType& value );
        PackableUint32<UnpackedType>& operator = ( PackableUint32<UnpackedType>&& ) = delete;
};


/***
 * 5. Packing and unpacking
 ***/

template <class UnpackedType >
void* PackableUint32<UnpackedType>::pack( void* buffer ) const
{
    std::uint32_t networkValue;

    // Cast the buffer.
    std::uint32_t* castedBuffer = static_cast< std::uint32_t* >( buffer );

    // Get the wrapper's inner value and translate it to network order.
    networkValue = static_cast< std::uint32_t >( this->value_ );
#if LITTLE_ENDIAN
    networkValue = ( (networkValue & 0xFF000000) >> 24 ) |
                    ( (networkValue & 0x00FF0000) >> 8 ) |
                    ( (networkValue & 0x0000FF00) << 8 ) |
                    ( (networkValue & 0x000000FF) << 24 );
#endif

    // Pack the wrapper's inner value into the buffer.
    *castedBuffer = networkValue;

    // Return a pointer to the next position in buffer.
    return static_cast< void* >( castedBuffer + 1 );

}


template <class UnpackedType>
const void* PackableUint32<UnpackedType>::unpack( const void* buffer )
{
    // Cast buffer to the UnpackedType type.
    const UnpackedType* castedBuffer = static_cast< const UnpackedType* >( buffer );

    // Unpack the wrapper's inner valued from the buffer and translate it from network order.
    this->value_ = static_cast< UnpackedType >( *castedBuffer );
#if LITTLE_ENDIAN
    this->value_ = ( (this->value_ & 0xFF000000) >> 24 ) |
            ( (this->value_ & 0x00FF0000) >> 8 ) |
            ( (this->value_ & 0x0000FF00) << 8 ) |
            ( (this->value_ & 0x000000FF) << 24 );
#endif

    // Return a pointer to the next position in buffer.
    return static_cast< const void* >( castedBuffer + 1 );
}


/***
 * 5. Operators
 ***/

template <class UnpackedType>
PackableUint32<UnpackedType>& PackableUint32<UnpackedType>::operator = ( const PackableUint32<UnpackedType>& b )
{
    PackableWrapper<UnpackedType>::operator =( b );

    return *this;
}


template <class UnpackedType>
PackableUint32<UnpackedType>& PackableUint32<UnpackedType>::operator = ( const UnpackedType& value )
{
    PackableWrapper<UnpackedType>::operator =( value );

    return *this;
}

} // namespace como

#endif // PACKABLE_UINT32_HPP
