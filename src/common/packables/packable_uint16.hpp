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

#include "packable_integer.hpp"

namespace como {

/*!
 * \class PackableUint16
 *
 * \brief Uint16 integer that can be packed into / unpacked from a given
 * buffer.
 * \tparam UnpackedType type of the inner value that eventually will be casted
 * to Uint16 before packing, or casted from Uint16 after unpacking.
 */
template <class UnpackedType>
class PackableUint16 : public PackableInteger<std::uint16_t, UnpackedType>
{
    /*! Integer value to be packed / unpacked */
    using PackableInteger<std::uint16_t, UnpackedType>::value_;

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        PackableUint16() = default;

        /*!
         * \brief Constructs a PackableUint16 from a given value
         * \param value for initializing the PackableUint16's inner value.
         */
        PackableUint16( const UnpackedType& value );

        /*! \brief Copy constructor */
        PackableUint16( const PackableUint16& ) = default;

        /*! \brief Move constructor */
        PackableUint16( PackableUint16&& ) = default;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~PackableUint16() = default;


        /***
         * 3. Getters
         ***/

        /*! \brief see PackableInteger::getPacketSize const */
        virtual std::uint16_t getPacketSize() const { return sizeof( std::uint16_t ); }


        /***
         * 4. Auxiliar methods
         ***/

        /*!
         * \brief flips the byte order (big endian or little endian) of the
         * given argument. Used when converting between local and network
         * endianess.
         * \param value whose bytes will be flipped.
         * \return the given value with its byte order flipped.
         */
        virtual std::uint16_t flipByteOrder( const std::uint16_t& value ) const;


        /***
         * 5. Operators
         ***/

        /*! Copy assignment operator */
        PackableUint16<UnpackedType>& operator = ( const PackableUint16<UnpackedType>& ) = default;

        /*! Move assignment operator */
        PackableUint16<UnpackedType>& operator = ( PackableUint16<UnpackedType>&& ) = default;
};

/***
 * 1. Construction
 ***/

template <class UnpackedType >
PackableUint16<UnpackedType>::PackableUint16( const UnpackedType& value )
    : PackableInteger<std::uint16_t, UnpackedType>( value )
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
