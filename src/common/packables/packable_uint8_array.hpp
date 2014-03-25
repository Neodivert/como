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

#ifndef PACKABLE_UINT8_ARRAY_HPP
#define PACKABLE_UINT8_ARRAY_HPP

#include "packable_array.hpp"

namespace como {

/*!
 * \class PackableUint8Array
 *
 * \brief Array of Uint8 that can be packed into or unpacked from a buffer.
 */
template <class UnpackedType, unsigned int ARRAY_SIZE>
class PackableUint8Array : public PackableArray<UnpackedType, ARRAY_SIZE>
{
    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        PackableUint8Array() = default;

        /*!
         * \brief Constructs a PackableUint8Array by copying a given plain
         * array.
         * \param values array of values for initializing the
         * PackableUint8Array.
         */
        PackableUint8Array( const UnpackedType* values ) : PackableArray<UnpackedType, ARRAY_SIZE>( values ){}

        /*! \brief Copy constructor */
        PackableUint8Array( const PackableUint8Array& ) = default;

        /*! \brief Move constructor */
        PackableUint8Array( PackableUint8Array&& ) = default;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~PackableUint8Array() = default;


        /***
         * 3. Getters
         ***/

        /*! \brief see PackableArray::getPacketSize const */
        virtual std::uint16_t getPacketSize() const { return sizeof( std::uint8_t ) * ARRAY_SIZE; }


        /***
         * 4. Packing and unpacking
         ***/

        /*!
         * \brief Packs the packable array into the given buffer.
         * \param buffer a pointer to the buffer where this packable array
         * will be packed into.
         * \return a pointer to the first position in the buffer after this
         * packable array's packed data.
         */
        virtual void* pack( void* buffer ) const;

        /*!
         * \brief Unpacks the packable array from the given buffer.
         * \param buffer a pointer to the buffer where this packable array
         * will be unpacked from.
         * \return a pointer to the first position in the buffer after this
         * packable array's packed data.
         */
        virtual const void* unpack( const void* buffer );

        /*!
         * \brief Unpacks array data from the given buffer AND THROW AN
         * EXCEPTION if it is different from this packable's array inner data.
         * \param buffer a pointer to the buffer where the data will be
         * unpacked from.
         * \return a pointer to the first position in the buffer after this
         * packable array's packed data.
         */
        virtual const void* unpack( const void* buffer ) const;


        /***
         * 6. Operators
         ***/

        /*! \brief Copy assignment operator */
        PackableUint8Array<UnpackedType, ARRAY_SIZE>& operator = ( const PackableUint8Array<UnpackedType, ARRAY_SIZE>& ) = default;
        //PackableUint8Array<UnpackedType, ARRAY_SIZE>& operator = ( const UnpackedType& value );

        /*! \brief Move assignment operator */
        PackableUint8Array<UnpackedType, ARRAY_SIZE>& operator = ( PackableUint8Array<UnpackedType, ARRAY_SIZE>&& ) = default;
};

// TODO: Leave this here?
typedef PackableUint8Array<std::uint8_t, 4> PackableRGBA;


/***
 * 5. Packing and unpacking
 ***/

template <class UnpackedType, unsigned int ARRAY_SIZE>
void* PackableUint8Array<UnpackedType, ARRAY_SIZE>::pack( void* buffer ) const
{
    unsigned int i;

    // Cast the buffer.
    std::uint8_t* castedBuffer = static_cast< std::uint8_t* >( buffer );

    // Pack the wrapper's inner values into the buffer.
    for( i=0; i<ARRAY_SIZE; i++ ){
        castedBuffer[i] = static_cast< std::uint8_t >( this->values_[i] );
    }

    // Return a pointer to the next position in buffer.
    return static_cast< void* >( castedBuffer + ARRAY_SIZE );
}


template <class UnpackedType, unsigned int ARRAY_SIZE>
const void* PackableUint8Array<UnpackedType, ARRAY_SIZE>::unpack( const void* buffer )
{
    unsigned int i;

    // Cast buffer to the UnpackedType type.
    const UnpackedType* castedBuffer = static_cast< const UnpackedType* >( buffer );

    // Unpack the wrapper's inner values from the buffer.
    for( i=0; i<ARRAY_SIZE; i++ ){
        this->values_[i] = static_cast< UnpackedType >( castedBuffer[i] );
    }

    // Return a pointer to the next position in buffer.
    return static_cast< const void* >( castedBuffer + ARRAY_SIZE );
}


template <class UnpackedType, unsigned int ARRAY_SIZE>
const void* PackableUint8Array<UnpackedType, ARRAY_SIZE>::unpack( const void* buffer ) const
{
    UnpackedType unpackedValue;

    unsigned int i;

    // Cast buffer to the UnpackedType type.
    const UnpackedType* castedBuffer = static_cast< const UnpackedType* >( buffer );

    // Unpack the wrapper's inner values from the buffer.
    for( i=0; i<ARRAY_SIZE; i++ ){
        unpackedValue = static_cast< UnpackedType >( castedBuffer[i] );

        if( unpackedValue != this->values_[i] ){
            throw std::runtime_error( "ERROR: Unpacked an unexpected PackableUint8Array" );
        }
    }

    // Return a pointer to the next position in buffer.
    return static_cast< const void* >( castedBuffer + ARRAY_SIZE );
}

} // namespace como

#endif // PACKABLE_UINT8_ARRAY_HPP
