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

#ifndef PACKABLE_ARRAY_WRAPPER_HPP
#define PACKABLE_ARRAY_WRAPPER_HPP

#include "packable.hpp"
#include <cstdint>

namespace como {

/*!
 * \class PackableArray
 *
 * \brief Base class for all arrays that can be packed into or unpacked from
 * a buffer.
 * \tparam UnpackedType type of the array elements.
 * \tparam ARRAY_SIZE array size (in number of elements).
 */
template <class UnpackedType, unsigned int ARRAY_SIZE>
class PackableArray : public Packable {
    protected:
        /*! Array of values to be packed / unpacked */
        UnpackedType values_[ARRAY_SIZE];

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        PackableArray<UnpackedType, ARRAY_SIZE>() = default;

        /*! \brief Constructs an packable array by copying a plain one */
        PackableArray<UnpackedType, ARRAY_SIZE>( const UnpackedType* values );

        /*! \brief Copy constructor */
        PackableArray<UnpackedType, ARRAY_SIZE>( const PackableArray<UnpackedType, ARRAY_SIZE>& ) = default;

        /*! \brief Move constructor */
        PackableArray<UnpackedType, ARRAY_SIZE>( PackableArray<UnpackedType, ARRAY_SIZE>&& ) = default;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~PackableArray() = default;


        /***
         * 3. Getters
         ***/

        /*!
         * \brief returns a pointer to the inner array.
         * \return a pointer to the inner array.
         */
        const UnpackedType* getValue() const ;

        /*! \brief see Packable::getPacketSize */
        virtual std::uint16_t getPacketSize() const = 0;

        /*!
         * \brief returns a reference to the indexed element.
         * \param index - the index of the requested array element.
         * \return a reference to the indexed element.
         */
        UnpackedType& operator []( unsigned int index );

        /*!
         * \brief returns a constant reference to the indexed element.
         * \param index - the index of the requested array element.
         * \return a reference to the indexed element.
         */
        const UnpackedType& operator []( unsigned int index ) const;


        /***
         * 4. Setters
         ***/

        /*!
         * \brief sets the values of the inner array.
         * \param values - a plain array of values to copy from.
         */
        void setValue( UnpackedType* values );


        /***
         * 5. Packing and unpacking
         ***/

        /*! \brief see Packable::pack */
        virtual void* pack( void* buffer ) const = 0;

        /*! \brief see Packable::unpack */
        virtual const void* unpack( const void* buffer ) = 0;

        /*! \brief see Packable::unpack const */
        virtual const void* unpack( const void* buffer ) const = 0;


        /***
         * 6. Operators
         ***/

        /*! \brief Assigns a plain array of values to this instance's inner array */
        PackableArray<UnpackedType, ARRAY_SIZE>& operator = ( const UnpackedType* value );

        /*! \brief Copy assignment operator */
        PackableArray<UnpackedType, ARRAY_SIZE>& operator = ( const PackableArray<UnpackedType, ARRAY_SIZE>& b );

        /*! \brief Move assignment operator */
        PackableArray<UnpackedType, ARRAY_SIZE>& operator = ( PackableArray<UnpackedType, ARRAY_SIZE>&& b );
};


/***
 * 1. Construction
 ***/

template <class UnpackedType, unsigned int ARRAY_SIZE>
PackableArray<UnpackedType, ARRAY_SIZE>::PackableArray( const UnpackedType* values )
{
    unsigned int i;

    for( i=0; i<ARRAY_SIZE; i++ ){
        values_[i] = values[i];
    }
}


/***
 * 3. Getters
 ***/

template <class UnpackedType, unsigned int ARRAY_SIZE>
const UnpackedType* PackableArray<UnpackedType, ARRAY_SIZE>::getValue() const
{
    return values_;
}


template <class UnpackedType, unsigned int ARRAY_SIZE>
UnpackedType& PackableArray<UnpackedType, ARRAY_SIZE>::operator []( unsigned int index )
{
    return values_[index];
}


template <class UnpackedType, unsigned int ARRAY_SIZE>
const UnpackedType& PackableArray<UnpackedType, ARRAY_SIZE>::operator []( unsigned int index ) const
{
    return values_[index];
}


/***
 * 4. Setters
 ***/

template <class UnpackedType, unsigned int ARRAY_SIZE>
void PackableArray<UnpackedType, ARRAY_SIZE>::setValue( UnpackedType* values )
{
    unsigned int i;

    for( i=0; i<ARRAY_SIZE; i++ ){
        values_[i] = values[i];
    }
}


/***
 * 6. Operators
 ***/

template <class UnpackedType, unsigned int ARRAY_SIZE>
PackableArray<UnpackedType, ARRAY_SIZE>& PackableArray<UnpackedType, ARRAY_SIZE>::operator = ( const PackableArray<UnpackedType, ARRAY_SIZE>& b )
{
    unsigned int i;

    if( this != &b ){
        for( i=0; i<ARRAY_SIZE; i++ ){
            values_[i] = b.values_[i];
        }
    }

    return *this;
}


template <class UnpackedType, unsigned int ARRAY_SIZE>
PackableArray<UnpackedType, ARRAY_SIZE>& PackableArray<UnpackedType, ARRAY_SIZE>::operator = ( const UnpackedType* values )
{
    unsigned int i;

    for( i=0; i<ARRAY_SIZE; i++ ){
        values_[i] = values[i];
    }

    return *this;
}

template <class UnpackedType, unsigned int ARRAY_SIZE>
PackableArray<UnpackedType, ARRAY_SIZE>& PackableArray<UnpackedType, ARRAY_SIZE>::operator = ( PackableArray<UnpackedType, ARRAY_SIZE>&& b )
{
    unsigned int i;

    if( this != &b ){
        for( i=0; i<ARRAY_SIZE; i++ ){
            values_[i] = b.values_[i];
        }
    }

    return *this;
}

} // namespace como

#endif // PACKABLE_ARRAY_WRAPPER_HPP
