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

template <class UnpackedType>
class PackableWrapper : public Packable {
    protected:
        UnpackedType value_;

    public:
        /***
         * 1. Construction
         ***/
        PackableWrapper() = default;
        PackableWrapper( const UnpackedType& value ) : value_( value ){}
        PackableWrapper( const PackableWrapper& ) = default;
        PackableWrapper( PackableWrapper&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~PackableWrapper() = default;


        /***
         * 3. Getters
         ***/
        UnpackedType getValue() const { return value_; }
        virtual std::uint16_t getPacketSize() const = 0;


        /***
         * 4. Setters
         ***/
        void setValue( UnpackedType value ){ value_ = value; }


        /***
         * 5. Packing and unpacking
         ***/
        virtual void* pack( void* buffer ) const = 0;
        virtual const void* unpack( const void* buffer ) = 0;


        /***
         * 6. Operators
         ***/
        PackableWrapper<UnpackedType>& operator = ( const PackableWrapper<UnpackedType>& b );
        PackableWrapper<UnpackedType>& operator = ( const UnpackedType& UnpackedType );
        PackableWrapper<UnpackedType>& operator = ( PackableWrapper<UnpackedType>&& ) = delete;
};


/***
 * 6. Operators
 ***/

template <class UnpackedType>
PackableWrapper<UnpackedType>& PackableWrapper<UnpackedType>::operator = ( const PackableWrapper<UnpackedType>& b )
{
    if( this != &b ){
        value_ = b.value_;
    }
    return *this;
}


template <class UnpackedType>
PackableWrapper<UnpackedType>& PackableWrapper<UnpackedType>::operator = ( const UnpackedType& value )
{
    value_ = value;

    return *this;
}

} // namespace como

#endif // PACKABLE_WRAPPER_HPP
