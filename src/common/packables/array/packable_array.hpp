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

#include <common/packables/abstract_packable_wrapper.hpp>
#include <common/packables/packable_wrapper.hpp>
#include <cstdint>
#include <array>
#include <memory>

namespace como {

// TODO: Use this?
//#define PACKABLE_ARRAY template <class ElementPackableType, class ElementPlainType, unsigned int ARRAY_SIZE >

template <class ElementPackableType, class ElementPlainType, unsigned int ARRAY_SIZE >
class PackableArray : public AbstractPackableWrapper< std::array< ElementPlainType, ARRAY_SIZE > >
{
    static_assert(
        std::is_base_of< PackableWrapper< ElementPlainType >, ElementPackableType>::value,
        "ElementPackableType must be a descendant of PackableWrapper< ElementPlainType >"
    );

    protected:
        std::array< ElementPackableType, ARRAY_SIZE > elements_;

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>() = default;

        /*! \brief Constructs an packable array by copying a plain one */
        PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>( const ElementPlainType* values );

        /*! \brief Copy constructor */
        PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>( const PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>& ) = default;

        /*! \brief Move constructor */
        PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>( PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>&& ) = default;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~PackableArray() = default;


        /***
         * 3. Getters
         ***/

        std::array< ElementPlainType, ARRAY_SIZE > getValue() const;

        /*!
         * \brief returns a pointer to the inner array.
         * \return a pointer to the inner array.
         */
        std::array< ElementPlainType, ARRAY_SIZE > getValues() const;


        /*! \brief see Packable::getPacketSize */
        virtual std::uint16_t getPacketSize() const;

        /*!
         * \brief returns a reference to the indexed element.
         * \param index - the index of the requested array element.
         * \return a reference to the indexed element.
         */
        ElementPackableType& operator []( unsigned int index );

        /*!
         * \brief returns a constant reference to the indexed element.
         * \param index - the index of the requested array element.
         * \return a reference to the indexed element.
         */
        const ElementPackableType& operator []( unsigned int index ) const;





        /***
         * 4. Setters
         ***/

        /*!
         * \brief sets the values of the inner array.
         * \param values - a plain array of values to copy from.
         */
        void setValues( ElementPlainType* values );


        void setValue( std::array< ElementPlainType, ARRAY_SIZE > values );

        void setValues( std::array< ElementPlainType, ARRAY_SIZE > values );





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

        /*! \brief Assigns a plain array of values to this instance's inner array */
        PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>& operator = ( const ElementPlainType* value );

        /*! \brief Copy assignment operator */
        PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>& operator = ( const PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>& b );

        /*! \brief Move assignment operator */
        PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>& operator = ( PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>&& b );
};


/***
 * 1. Construction
 ***/

template <class ElementPackableType, class ElementPlainType, unsigned int ARRAY_SIZE >
PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>::PackableArray( const ElementPlainType* values )
{
    unsigned int i;

    for( i=0; i<ARRAY_SIZE; i++ ){
        elements_[i] = values[i];
    }
}


/***
 * 3. Getters
 ***/

template <class ElementPackableType, class ElementPlainType, unsigned int ARRAY_SIZE >
std::uint16_t PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>::getPacketSize() const
{
    return elements_[0].getPacketSize() * ARRAY_SIZE;
}


template <class ElementPackableType, class ElementPlainType, unsigned int ARRAY_SIZE >
std::array< ElementPlainType, ARRAY_SIZE > PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>::getValue() const
{
    return getValues();
}

template <class ElementPackableType, class ElementPlainType, unsigned int ARRAY_SIZE >
std::array< ElementPlainType, ARRAY_SIZE > PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>::getValues() const
{
    unsigned int i;
    std::array< ElementPlainType, ARRAY_SIZE > plainElements;

    for( i=0; i<ARRAY_SIZE; i++ ){
        plainElements[i] = elements_[i].getValue();
    }

    return plainElements;
}


template <class ElementPackableType, class ElementPlainType, unsigned int ARRAY_SIZE >
ElementPackableType& PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>::operator []( unsigned int index )
{
    return elements_[index];
}


template <class ElementPackableType, class ElementPlainType, unsigned int ARRAY_SIZE >
const ElementPackableType& PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>::operator []( unsigned int index ) const
{
    return elements_[index];
}


/***
 * 4. Setters
 ***/

template <class ElementPackableType, class ElementPlainType, unsigned int ARRAY_SIZE >
void PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>::setValues( ElementPlainType* values )
{
    unsigned int i;

    for( i=0; i<ARRAY_SIZE; i++ ){
        elements_[i] = values[i];
    }
}


template <class ElementPackableType, class ElementPlainType, unsigned int ARRAY_SIZE >
void PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>::setValue( std::array< ElementPlainType, ARRAY_SIZE > values )
{
    setValues( values );
}


template <class ElementPackableType, class ElementPlainType, unsigned int ARRAY_SIZE >
void PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>::setValues( std::array< ElementPlainType, ARRAY_SIZE > values )
{
    unsigned int i;

    for( i=0; i<ARRAY_SIZE; i++ ){
        elements_[i] = values[i];
    }
}


/***
 * 5. Packing and unpacking
 ***/

template <class ElementPackableType, class ElementPlainType, unsigned int ARRAY_SIZE >
void* PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>::pack( void* buffer ) const
{
    unsigned int i;

    for( i=0; i<ARRAY_SIZE; i++ ){
        buffer = elements_[i].pack( buffer );
    }

    return buffer;
}


template <class ElementPackableType, class ElementPlainType, unsigned int ARRAY_SIZE >
const void* PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>::unpack( const void* buffer )
{
    unsigned int i;

    for( i=0; i<ARRAY_SIZE; i++ ){
        buffer = elements_[i].unpack( buffer );
    }

    return buffer;
}


template <class ElementPackableType, class ElementPlainType, unsigned int ARRAY_SIZE >
const void* PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>::unpack( const void* buffer ) const
{
    unsigned int i;

    for( i=0; i<ARRAY_SIZE; i++ ){
        buffer = elements_[i].unpack( buffer );
    }

    return buffer;
}


/***
 * 6. Operators
 ***/

template <class ElementPackableType, class ElementPlainType, unsigned int ARRAY_SIZE >
PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>& PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>::operator = ( const PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>& b )
{
    unsigned int i;

    if( this != &b ){
        for( i=0; i<ARRAY_SIZE; i++ ){
            elements_[i] = b.elements_[i];
        }
    }

    return *this;
}


template <class ElementPackableType, class ElementPlainType, unsigned int ARRAY_SIZE >
PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>& PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>::operator = ( const ElementPlainType* values )
{
    unsigned int i;

    for( i=0; i<ARRAY_SIZE; i++ ){
        elements_[i] = values[i];
    }

    return *this;
}

template <class ElementPackableType, class ElementPlainType, unsigned int ARRAY_SIZE >
PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>& PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>::operator = ( PackableArray<ElementPackableType, ElementPlainType, ARRAY_SIZE>&& b )
{
    unsigned int i;

    if( this != &b ){
        for( i=0; i<ARRAY_SIZE; i++ ){
            elements_[i] = b.elements_[i];
        }
    }

    return *this;
}

} // namespace como

#endif // PACKABLE_ARRAY_WRAPPER_HPP
