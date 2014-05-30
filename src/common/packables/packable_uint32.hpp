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

#include "packable_integer.hpp"

namespace como {

/*!
 * \class PackableUint32
 *
 * \brief Uint16 integer that can be packed into / unpacked from a given
 * buffer.
 * \tparam UnpackedType type of the inner value that eventually will be casted
 * to Uint32 before packing, or casted from Uint32 after unpacking.
 */
template <class UnpackedType>
class PackableUint32 : public PackableInteger<std::uint32_t, UnpackedType>
{
    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        PackableUint32() = default;

        /*!
         * \brief Constructs a PackableUint32 by copying the given value.
         * \param value for initializing the PackableUint32's inner value.
         */
        PackableUint32( const UnpackedType& value );

        /*! \brief Copy constructor */
        PackableUint32( const PackableUint32& ) = default;

        /*! \brief Move constructor */
        PackableUint32( PackableUint32&& ) = default;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~PackableUint32() = default;


        /***
         * 3. Getters
         ***/

        /*! \brief see PackableInteger::getPacketSize const */
        virtual PacketSize getPacketSize() const { return sizeof( std::uint32_t ); }


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
        virtual std::uint32_t flipByteOrder( const std::uint32_t& value ) const;


        /***
         * 5. Operators
         ***/

        /*! \brief Copy assignment operator */
        PackableUint32<UnpackedType>& operator = ( const PackableUint32<UnpackedType>& ) = default;

        /*! \brief Move assignment operator */
        PackableUint32<UnpackedType>& operator = ( PackableUint32<UnpackedType>&& ) = default;
};


/***
 * 1. Construction
 ***/

template <class UnpackedType >
PackableUint32<UnpackedType>::PackableUint32( const UnpackedType& value )
    : PackableInteger<std::uint32_t, UnpackedType>( value )
{}


/***
 * 4. Auxiliar methods
 ***/

template <class UnpackedType >
std::uint32_t PackableUint32<UnpackedType>::flipByteOrder( const std::uint32_t& value ) const
{
    return ( (value & 0xFF000000) >> 24 ) |
            ( (value & 0x00FF0000) >> 8 ) |
            ( (value & 0x0000FF00) << 8 ) |
            ( (value & 0x000000FF) << 24 );
}

} // namespace como

#endif // PACKABLE_UINT32_HPP
