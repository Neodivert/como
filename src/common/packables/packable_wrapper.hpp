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

#ifndef PACKABLE_WRAPPER_HPP
#define PACKABLE_WRAPPER_HPP

#include "packable.hpp"

namespace como {

template <class PackedValue, class UnpackedValue>
class PackableWrapper : public Packable {
    protected:
        UnpackedValue value_;

    public:
        /***
         * 1. Construction
         ***/
        PackableWrapper() = default;
        PackableWrapper( const UnpackedValue& value ) : value_( value ){}
        PackableWrapper( const PackableWrapper& ) = default;
        PackableWrapper( PackableWrapper&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~PackableWrapper() = default;


        /***
         * 3. Getters
         ***/
        UnpackedValue getValue() const { return value_; }
        virtual std::uint16_t getPacketSize() const { return sizeof( PackedValue ); }


        /***
         * 4. Setters
         ***/
        void setValue( UnpackedValue value ){ value_ = value; }


        /***
         * 5. Packing and unpacking
         ***/
        virtual void* pack( void* buffer ) const ;
        virtual const void* unpack( const void* buffer ) ;


        /***
         * 6. Operators
         ***/
        PackableWrapper<PackedValue, UnpackedValue>& operator = ( const PackableWrapper<PackedValue, UnpackedValue>& b );
        PackableWrapper<PackedValue, UnpackedValue>& operator = ( PackableWrapper<PackedValue, UnpackedValue>&& ) = delete;
};


/***
 * 5. Packing and unpacking
 ***/

template <class PackedValue, class UnpackedValue >
void* PackableWrapper< PackedValue, UnpackedValue >::pack( void* buffer ) const
{
    // Cast the buffer to the PackedValue type.
    PackedValue* castedBuffer = static_cast< PackedValue* >( buffer );

    // Pack the wrapper's inner value into the buffer.
    *castedBuffer = static_cast< PackedValue >( value_ );

    // Return a pointer to the next position in buffer.
    return static_cast< void* >( castedBuffer + 1 );
}


template <class PackedValue, class UnpackedValue>
const void* PackableWrapper< PackedValue, UnpackedValue >::unpack( const void* buffer )
{
    // Cast buffer to the UnpackedValue type.
    const UnpackedValue* castedBuffer = static_cast< const UnpackedValue* >( buffer );

    // Unpack the wrapper's inner valued from the buffer.
    value_ = *castedBuffer;

    // Return a pointer to the next position in buffer.
    return static_cast< const void* >( castedBuffer + 1 );
}


/***
 * 6. Operators
 ***/

template <class PackedValue, class UnpackedValue>
PackableWrapper<PackedValue, UnpackedValue>& PackableWrapper<PackedValue, UnpackedValue>::operator = ( const PackableWrapper< PackedValue, UnpackedValue >& b )
{
    if( this != &b ){
        value_ = b.value_;
    }
    return *this;
}



} // namespace como

#endif // PACKABLE_WRAPPER_HPP
