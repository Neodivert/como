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
class PackableUint16 : public PackableWrapper<std::uint16_t, UnpackedType>
{
    using PackableWrapper<std::uint16_t, UnpackedType>::value_;

    public:
        /***
         * 1. Construction
         ***/
        PackableUint16() = default;
        PackableUint16( const UnpackedType& value );
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
         * 4. Auxiliar methods
         ***/
        virtual std::uint16_t flipByteOrder( const std::uint16_t& value ) const;


        /***
         * 5. Operators
         ***/
        PackableUint16<UnpackedType>& operator = ( const PackableUint16<UnpackedType>& ) = default;
        PackableUint16<UnpackedType>& operator = ( PackableUint16<UnpackedType>&& ) = default;
};

/***
 * 1. Construction
 ***/

template <class UnpackedType >
PackableUint16<UnpackedType>::PackableUint16( const UnpackedType& value )
    : PackableWrapper<std::uint16_t, UnpackedType>( value )
{}


/***
 * 4. Auxiliar methods
 ***/

template <class UnpackedType >
std::uint16_t PackableUint16<UnpackedType>::flipByteOrder( const std::uint16_t& value ) const
{
    return ( (value & 0xFF00) >> 8 ) | ((value & 0x00FF ) << 8);
}

} // namespace como

#endif // PACKABLE_UINT16_HPP
