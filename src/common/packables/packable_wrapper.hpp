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

#ifndef PACKABLE_WRAPPER_HPP
#define PACKABLE_WRAPPER_HPP

#include <common/packables/packable.hpp>

namespace como {

template <class PlainType>
class PackableWrapper : public Packable
{
    private:
        PlainType value_;

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        PackableWrapper() = default;

        /*! \brief Constructs a PackableWrapper around the given value. */
        PackableWrapper( const PlainType& value ) : value_( value ){}

        /*! \brief Copy constructor */
        PackableWrapper( const PackableWrapper& ) = default;

        /*! \brief Move constructor */
        PackableWrapper( PackableWrapper&& ) = default;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~PackableWrapper() = default;


        /***
         * 3. Getters
         ***/

        /*! \brief Returns the valued held by this PackableWrapper */
        PlainType getValue() const { return value_; }

        /*! \brief see Packable::getPacketSize const */
        virtual std::uint16_t getPacketSize() const = 0;


        /***
         * 4. Setters
         ***/

        /*! \brief Set this PackableWrapper's inner value */
        void setValue( PlainType value ){ this->value_ = value; }


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

        /*! \brief Assigns the given value to this PackableWrapper */
        PackableWrapper<PlainType>& operator = ( const PlainType& value );

        /*! \brief Copy assignment operator */
        PackableWrapper<PlainType>& operator = ( const PackableWrapper<PlainType>& ) = default;

        /*! \brief Move assignment operator */
        PackableWrapper<PlainType>& operator = ( PackableWrapper<PlainType>&& ) = default;
};


/***
 * 7. Operators
 ***/

/*! \brief Assigns the given value to this PackableWrapper */
template < class PlainType >
PackableWrapper<PlainType>& PackableWrapper<PlainType>::operator = ( const PlainType& value )
{
    value_ = value;
}


} // namespace como

#endif // PACKABLEWRAPPER_HPP
