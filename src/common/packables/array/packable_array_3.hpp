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

#ifndef PACKABLE_ARRAY_3_HPP
#define PACKABLE_ARRAY_3_HPP

#include <common/packables/array/packable_array.hpp>

namespace como {

template <class ElementPackableType, class ElementPlainType>
class PackableArray3 : public PackableArray< ElementPackableType, ElementPlainType, 3 >
{
    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        PackableArray3<ElementPackableType, ElementPlainType>() = default;

        /*! \brief Constructs an packable array by copying a plain one */
        PackableArray3<ElementPackableType, ElementPlainType>( const ElementPlainType& x, const ElementPlainType& y, const ElementPlainType& z );

        /*! \brief Copy constructor */
        PackableArray3<ElementPackableType, ElementPlainType>( const PackableArray3<ElementPackableType, ElementPlainType>& ) = default;

        /*! \brief Move constructor */
        PackableArray3<ElementPackableType, ElementPlainType>( PackableArray3<ElementPackableType, ElementPlainType>&& ) = default;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~PackableArray3() = default;



        /***
         * 4. Setters
         ***/

        void setValues( const ElementPlainType& x, const ElementPlainType& y, const ElementPlainType& z );


        /***
         * 6. Operators
         ***/

        /*! \brief Assigns a plain array of values to this instance's inner array */
        PackableArray3<ElementPackableType, ElementPlainType>& operator = ( const ElementPlainType* values );

        /*! \brief Copy assignment operator */
        PackableArray3<ElementPackableType, ElementPlainType>& operator = ( const PackableArray3<ElementPackableType, ElementPlainType>& ) = default;

        /*! \brief Move assignment operator */
        PackableArray3<ElementPackableType, ElementPlainType>& operator = ( PackableArray3<ElementPackableType, ElementPlainType>&& ) = default;
};


/***
 * 1. Construction
 ***/

template <class ElementPackableType, class ElementPlainType>
PackableArray3<ElementPackableType, ElementPlainType>::PackableArray3( const ElementPlainType& x, const ElementPlainType& y, const ElementPlainType& z )
{
    setValues( x, y, z );
}


/***
 * 4. Setters
 ***/

template <class ElementPackableType, class ElementPlainType>
void PackableArray3<ElementPackableType, ElementPlainType>::setValues( const ElementPlainType& x, const ElementPlainType& y, const ElementPlainType& z )
{
    this->elements_[0] = x;
    this->elements_[1] = y;
    this->elements_[2] = z;
}


/***
 * 6. Operators
 ***/

template <class ElementPackableType, class ElementPlainType>
PackableArray3<ElementPackableType, ElementPlainType>& PackableArray3<ElementPackableType, ElementPlainType>::operator = ( const ElementPlainType* values )
{
    return PackableArray< ElementPackableType, ElementPlainType, 3 >::operator =( values );
}

} // namespace como

#endif // PACKABLE_ARRAY_3_HPP
