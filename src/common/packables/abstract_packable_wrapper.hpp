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

#ifndef ABSTRACT_PACKABLE_WRAPPER_HPP
#define ABSTRACT_PACKABLE_WRAPPER_HPP

#include "packable.hpp"

namespace como {

template <class PlainType>
class AbstractPackableWrapper : public Packable
{
    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        AbstractPackableWrapper() = default;

        /*! \brief Copy constructor */
        AbstractPackableWrapper( const AbstractPackableWrapper& ) = default;

        /*! \brief Move constructor */
        AbstractPackableWrapper( AbstractPackableWrapper&& ) = default;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        virtual ~AbstractPackableWrapper() = default;


        /***
         * 3. Getters
         ***/

        /*! \brief Returns the valued held by this AbstractPackableWrapper */
        virtual PlainType getValue() const = 0;

        /*! \brief see Packable::getPacketSize const */
        virtual PacketSize getPacketSize() const = 0;


        /***
         * 4. Setters
         ***/

        /*! \brief Set this AbstractPackableWrapper's inner value */
        virtual void setValue( const PlainType& value ) = 0;


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
         * 7. Operators
         ***/

        /*! \brief Assigns the given value to this AbstractPackableWrapper */
        AbstractPackableWrapper<PlainType>& operator = ( const PlainType& value );

        /*! \brief Copy assignment operator */
        AbstractPackableWrapper<PlainType>& operator = ( const AbstractPackableWrapper<PlainType>& ) = default;

        /*! \brief Move assignment operator */
        AbstractPackableWrapper<PlainType>& operator = ( AbstractPackableWrapper<PlainType>&& ) = default;
};

/***
 * 7. Operators
 ***/

template <class PlainType>
AbstractPackableWrapper<PlainType>& AbstractPackableWrapper<PlainType>::operator = ( const PlainType& value )
{
    setValue( value );
}

} // namespace como

#endif // ABSTRACT_PACKABLE_WRAPPER_HPP
