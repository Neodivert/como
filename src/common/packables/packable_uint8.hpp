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

#include "packable_integer.hpp"
#include <stdexcept>

namespace como {

/*!
 * \class PackableUint8
 *
 * \brief Uint8 integer that can be packed into / unpacked from a given
 * buffer.
 * \tparam UnpackedType type of the inner value that eventually will be casted
 * to Uint8 before packing, or casted from Uint8 after unpacking.
 */
template <class UnpackedType>
class PackableUint8 : public PackableInteger<std::uint8_t, UnpackedType>
{
    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        PackableUint8() = default;

        /*!
         * \brief Constructs a PackableUint8 by copying the given value.
         * \param value for initializing the PackableUint8's inner value.
         */
        PackableUint8( const UnpackedType& value );

        /*! \brief Copy constructor */
        PackableUint8( const PackableUint8& ) = default;

        /*! \brief Move constructor */
        PackableUint8( PackableUint8&& ) = default;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~PackableUint8() = default;


        /***
         * 3. Getters
         ***/

        /*! \brief see PackableInteger::getPacketSize const */
        virtual std::uint16_t getPacketSize() const { return sizeof( std::uint8_t ); }


        /***
         * 4. Auxiliar methods
         ***/

        /*!
         * \brief emtpy method for compatibility with PackableInteger's
         * interface.
         * \param value whose bytes "will be flipped".
         * \return the given value without changes ( :-) ).
         */
        virtual std::uint8_t flipByteOrder( const std::uint8_t& value ) const;


        /***
         * 5. Operators
         ***/

        /*! \brief Copy assignment operator */
        PackableUint8<UnpackedType>& operator = ( const PackableUint8<UnpackedType>& ) = default;

        /*! \brief Move assignment operator */
        PackableUint8<UnpackedType>& operator = ( PackableUint8<UnpackedType>&& ) = default;
};


/***
 * 1. Construction
 ***/

template <class UnpackedType >
PackableUint8<UnpackedType>::PackableUint8( const UnpackedType& value )
    : PackableInteger<std::uint8_t, UnpackedType>( value )
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
