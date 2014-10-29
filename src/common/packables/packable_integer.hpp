/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published by
 * the Free Software Foundation.
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

#include <common/packables/packable_wrapper.hpp>
#include <cstdint>
#include <cassert> // TODO: Remove

namespace como {

/*!
 * \class PackableInteger
 *
 * \brief Base class for all types of variables that can be packed as integers
 * into or unpacked from a buffer.
 * \tparam PackedType integral type of data once packed in the buffer.
 * \tparam UnpackedType type of data that eventually will be casted to / from
 * PackedType when packing / unpacking.
 */
template <class PackedType, class UnpackedType>
class PackableInteger : public PackableWrapper< UnpackedType > {
    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        PackableInteger() = default;

        /*!
         * \brief Constructs a PackableInteger from the given value.
         * \param value value to initialize PackableInteger with.
         */
        PackableInteger( const UnpackedType& value ) : PackableWrapper< UnpackedType >( value ){}

        /*! \brief Copy constructor */
        PackableInteger( const PackableInteger& ) = default;

        /*! \brief Move constructor */
        PackableInteger( PackableInteger&& ) = default;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        virtual ~PackableInteger() = default;


        /***
         * 3. Getters
         ***/

        virtual PacketSize getPacketSize() const;

        static PacketSize packetSize();


        /***
         * 4. Packing and unpacking
         ***/

        /*! \brief see Packable::pack */
        virtual void* pack( void* buffer ) const;

        /*! \brief see Packable::unpack */
        virtual const void* unpack( const void* buffer );

        /*! \brief see Packable::unpack const */
        virtual const void* unpack( const void* buffer ) const;


        /***
         * 6. Operators
         ***/

        /*! \brief Assigns the given value to this PackableInteger */
        PackableInteger<PackedType, UnpackedType>& operator = ( const UnpackedType& value );

        /*! \brief Copy assignment operator */
        PackableInteger<PackedType, UnpackedType>& operator = ( const PackableInteger<PackedType, UnpackedType>& ) = default;

        /*! \brief Move assignment operator */
        PackableInteger<PackedType, UnpackedType>& operator = ( PackableInteger<PackedType, UnpackedType>&& ) = default;
};

template <class UnpackedType>
using PackableUint8 = PackableInteger< std::uint8_t, UnpackedType >;

template <class UnpackedType>
using PackableUint16 = PackableInteger< std::uint16_t, UnpackedType >;

template <class UnpackedType>
using PackableUint32 = PackableInteger< std::uint32_t, UnpackedType >;

/***
 * 5. Auxiliar methods
 ***/

inline std::uint8_t flipByteOrder( const std::uint8_t& value ){
    return value;
}

inline std::uint16_t flipByteOrder( const std::uint16_t& value ){
    return ( (value & 0xFF00) >> 8 ) | ((value & 0x00FF ) << 8);
}

inline std::uint32_t flipByteOrder( const std::uint32_t& value ){
    return ( (value & 0xFF000000) >> 24 ) |
            ( (value & 0x00FF0000) >> 8 ) |
            ( (value & 0x0000FF00) << 8 ) |
            ( (value & 0x000000FF) << 24 );
}


/***
 * 3. Getters
 ***/

template <class PackedType, class UnpackedType>
PacketSize PackableInteger<PackedType, UnpackedType>::getPacketSize() const
{
    return sizeof( PackedType );
}


template <class PackedType, class UnpackedType>
PacketSize PackableInteger<PackedType, UnpackedType>::packetSize()
{
    return sizeof( PackedType );
}


/***
 * 4. Packing and unpacking
 ***/

template <class PackedType, class UnpackedType>
void* PackableInteger<PackedType, UnpackedType>::pack( void* buffer ) const
{
    PackedType networkValue;

    // Cast the buffer.
    PackedType* castedBuffer = static_cast< PackedType* >( buffer );

    // Get the wrapper's inner value.
    networkValue = static_cast< PackedType >( this->getValue() );

    // If necessary, translate value to network order.
#if LITTLE_ENDIAN
    networkValue = flipByteOrder( networkValue );
#endif

    // Pack the wrapper's inner value into the buffer.
    assert( castedBuffer != nullptr );
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
    this->setValue( static_cast< UnpackedType >( networkValue ) );

    // Return a pointer to the next position in buffer.
    return static_cast< const void* >( castedBuffer + 1 );
}


template <class PackedType, class UnpackedType>
const void* PackableInteger<PackedType, UnpackedType>::unpack( const void* buffer ) const
{
    PackedType networkValue;
    char errorMessage[128];

    // Cast buffer to the UnpackedType type.
    const PackedType* castedBuffer = static_cast< const PackedType* >( buffer );

    // Unpack the wrapper's inner value from the buffer.
    networkValue = static_cast< PackedType >( *castedBuffer );

    // If necessary, translate value from network order.
#if LITTLE_ENDIAN
    networkValue = flipByteOrder( networkValue );
#endif

    // If the unpacked value isn't the expected one, throw an exception.
    if( static_cast< UnpackedType >( networkValue ) != this->getValue() ){
        sprintf( errorMessage,
                 "ERROR: Unpacked an unexpected unsigned integer. Expected value (%u), unpacked value (%u)",
                 static_cast< PackedType >( this->getValue() ),
                 static_cast< PackedType >( networkValue ) );
        throw std::runtime_error( errorMessage );
    }

    // Return a pointer to the next position in buffer.
    return static_cast< const void* >( castedBuffer + 1 );
}


/***
 * 6. Operators
 ***/

template <class PackedType, class UnpackedType>
PackableInteger<PackedType, UnpackedType>& PackableInteger<PackedType, UnpackedType>::operator = ( const UnpackedType& value )
{
    PackableWrapper< UnpackedType >::operator = ( value );

    return *this;
}

} // namespace como

#endif // PACKABLE_INTEGER_HPP
