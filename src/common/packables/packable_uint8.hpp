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

#ifndef PACKABLE_UINT8_HPP
#define PACKABLE_UINT8_HPP

#include "packable_wrapper.hpp"
#include <stdexcept>

namespace como {

template <class UnpackedType>
class PackableUint8 : public PackableWrapper<std::uint8_t, UnpackedType>
{
    public:
        /***
         * 1. Construction
         ***/
        PackableUint8() = default;
        PackableUint8( const UnpackedType& value );
        PackableUint8( const PackableUint8& ) = default;
        PackableUint8( PackableUint8&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~PackableUint8() = default;


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const { return sizeof( std::uint8_t ); }


        /***
         * 4. Auxiliar methods
         ***/
        virtual std::uint8_t flipByteOrder( const std::uint8_t& value ) const;


        /***
         * 5. Operators
         ***/
        PackableUint8<UnpackedType>& operator = ( const PackableUint8<UnpackedType>& ) = default;
        PackableUint8<UnpackedType>& operator = ( PackableUint8<UnpackedType>&& ) = default;
};


/***
 * 1. Construction
 ***/

template <class UnpackedType >
PackableUint8<UnpackedType>::PackableUint8( const UnpackedType& value )
    : PackableWrapper<std::uint8_t, UnpackedType>( value )
{}


/***
 * 4. Auxiliar methods
 ***/

template <class UnpackedType >
std::uint8_t PackableUint8<UnpackedType>::flipByteOrder( const std::uint8_t& value ) const
{
    return value;
}

} // namespace como

#endif // PACKABLE_UINT8_HPP
