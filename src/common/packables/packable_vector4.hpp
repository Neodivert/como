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

#ifndef PACKABLE_VECTOR4_HPP
#define PACKABLE_VECTOR4_HPP

#include "packable.hpp"

namespace como {

const unsigned int PACK_STRING_SIZE = 32;


/*!
 * \class PackableUint8
 *
 * \brief Vector of four float values that can be packed into / unpacked from
 * a given buffer.
 */
class PackableVector4 : public Packable
{
    private:
        /*! Values to be packed / unpacked */
        float values_[4];

        /*! Previous values are writen to / read from this string when
         * packing / unpacking */
        char packString_[PACK_STRING_SIZE];

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        PackableVector4();

        /*! \brief Constructs a PackableVector4 from the given values  */
        PackableVector4( float x, float y, float z, float w );

        /*!
         * \brief Constructs a PackableVector4 from the given array of
         * values
        */
        PackableVector4( const float* values );

        /*! \brief Copy constructor */
        PackableVector4( const PackableVector4& b );

        /*! \brief Move constructor */
        PackableVector4( PackableVector4&& b );


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~PackableVector4() = default;


        /***
         * 3. Getters
         ***/

        /*! \brief see Packable::getPacketSize() const */
        virtual std::uint16_t getPacketSize() const;

        /*!
         * \brief returns a pointer to the plain array held by this
         * object
        */
        const float* getValue() const ;

        /*!
         * \brief returns an element of the plain array held by this
         * object.
         * \param index index of the desired element.
         * \return the indexed element.
         */
        const float& operator []( unsigned int index ) const;


        /***
         * 4. Setters
         ***/

        /*!
         * \brief Assign the given values to the plain array held by this
         * object.
         */
        void setValue( float x, float y, float z, float w );


        /***
         * 5. Packing and unpacking
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

        /*!
         * \brief Assigns the given plain array to the one held by this
         * object
         */
        PackableVector4& operator = ( const float* values );

        /*! \brief Copy assignment operator */
        PackableVector4& operator = ( const PackableVector4& b );

        /*! \brief Move assignment operator */
        PackableVector4& operator = ( PackableVector4&& ) = delete;
};

} // namespace como

#endif // PACKABLE_VECTOR4_HPP
