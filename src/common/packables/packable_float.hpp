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

#ifndef PACKABLE_FLOAT_HPP
#define PACKABLE_FLOAT_HPP

#include <common/packables/packable_wrapper.hpp>

namespace como {

class PackableFloat : public PackableWrapper< float >
{
    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        PackableFloat() = default;

        /*!
         * \brief Constructs a PackableFloat from the given value.
         * \param value value to initialize PackableFloat with.
         */
        PackableFloat( float value );

        /*! \brief Copy constructor */
        PackableFloat( const PackableFloat& ) = default;

        /*! \brief Move constructor */
        PackableFloat( PackableFloat&& ) = default;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~PackableFloat() = default;


        /***
         * 3. Packing and unpacking
         ***/

        /*! \brief see Packable::pack */
        virtual void* pack( void* buffer ) const;

        /*! \brief see Packable::unpack */
        virtual const void* unpack( const void* buffer );

        /*! \brief see Packable::unpack const */
        virtual const void* unpack( const void* buffer ) const;


        /***
         * 4. Getters
         ***/

        /*! \brief see Packable::getPacketSize const */
        virtual PacketSize getPacketSize() const;


        /***
         * 5. Operators
         ***/

        /*! \brief Copy assignment operator */
        PackableFloat& operator = ( const PackableFloat& ) = default;

        /*! \brief Move assignment operator */
        PackableFloat& operator = ( PackableFloat&& ) = default;
};

}

#endif // PACKABLE_FLOAT_HPP
