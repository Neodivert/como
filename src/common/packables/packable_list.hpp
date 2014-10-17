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

#ifndef PACKABLE_LIST_HPP
#define PACKABLE_LIST_HPP

#include <common/packables/packable.hpp>
#include <common/packables/abstract_packable_wrapper.hpp>
#include <common/packables/packable_packet_size.hpp>
#include <list>
#include <memory>

namespace como {

template <class PackableElementType, class PlainElementType>
class PackableList : public AbstractPackableWrapper< std::list<PlainElementType> > {
    static_assert(
            std::is_base_of< AbstractPackableWrapper< PlainElementType >, PackableElementType>::value,
            "PackableElementType must be a descendant of AbstractPackableWrapper< PlainElementType >" );

    public:
        /***
         * 1. Construction
         ***/
        PackableList() = default;
        PackableList( const PackableList& ) = default;
        PackableList( PackableList&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~PackableList() = default;


        /***
         * 3. Elements management
         ***/
        void addElement( const PlainElementType& plainElement );
        void clear();


        /***
         * 4. Getters
         ***/
        virtual PacketSize getPacketSize() const;
        virtual std::uint8_t size() const;
        virtual std::list<PlainElementType> getValue() const;


        /***
         * 5. Setters
         ***/
        virtual void setValue(const std::list<PlainElementType> &value);


        /***
         * 6. Packing and unpacking
         ***/
        virtual void* pack( void* buffer ) const;
        virtual const void* unpack( const void* buffer );
        virtual const void* unpack( const void* buffer ) const;


        /***
         * 7. Operators
         ***/
        PackableList& operator = ( const PackableList& ) = delete;
        PackableList& operator = ( PackableList&& ) = delete;


    private:
        std::list< PackableElementType > elements_;
};


/***
 * 3. Elements management
 ***/

template <class PackableElementType, class PlainElementType>
void PackableList<PackableElementType, PlainElementType>::addElement( const PlainElementType &plainElement )
{
    elements_.push_back( PackableElementType( plainElement ) );
}


template <class PackableElementType, class PlainElementType>
void PackableList<PackableElementType, PlainElementType>::clear()
{
    elements_.clear();
}


/***
 * 4. Getters
 ***/

template <class PackableElementType, class PlainElementType>
PacketSize PackableList<PackableElementType, PlainElementType>::getPacketSize() const
{
    PacketSize packetSize = 0;

    for( const PackableElementType& element : elements_ ){
        packetSize += element.getPacketSize();
    }

    return packetSize;
}


template <class PackableElementType, class PlainElementType>
std::uint8_t PackableList<PackableElementType, PlainElementType>::size() const
{
    return this->elements_.size();
}


template <class PackableElementType, class PlainElementType>
std::list<PlainElementType> PackableList<PackableElementType, PlainElementType>::getValue() const
{
    std::list<PlainElementType> plainList;

    for( const PackableElementType& element : elements_ ){
        plainList.push_back( element.getValue() );
    }

    return plainList;
}


/***
 * 5. Setters
 ***/

template <class PackableElementType, class PlainElementType>
void PackableList<PackableElementType, PlainElementType>::setValue( const std::list<PlainElementType> &plainList )
{
    clear();

    for( const PlainElementType& plainElement : plainList ){
        PackableElementType newElement( plainElement );
        elements_.push_back( newElement );
    }
}


/***
 * 6. Packing and unpacking
 ***/

template <class PackableElementType, class PlainElementType>
void *PackableList<PackableElementType, PlainElementType>::pack( void *buffer ) const
{
    // Pack the number of elements.
    PackableUint8< std::uint8_t > listSize = size();
    buffer = listSize.pack( buffer );

    // Pack the elements.
    for( const PackableElementType& element : elements_ ){
        buffer = element.pack( buffer );
    }

    return buffer;
}


template <class PackableElementType, class PlainElementType>
const void *PackableList<PackableElementType, PlainElementType>::unpack( const void *buffer )
{
    unsigned int i;

    // Unpack the number of elements.
    PackableUint8< std::uint8_t > listSize;
    buffer = listSize.unpack( buffer );

    // Unpack the elements.
    for( i = 0; i < listSize.getValue(); i++ ){
        PackableElementType newElement;
        buffer = newElement.unpack( buffer );
        elements_.push_back( newElement );
    }

    return buffer;
}


template <class PackableElementType, class PlainElementType>
const void *PackableList<PackableElementType, PlainElementType>::unpack( const void *buffer ) const
{
    const PackableUint8< std::uint8_t > listSize( size() );
    buffer = listSize.unpack( buffer );

    for( const PackableElementType& element : elements_ ){
        buffer = element.unpack( buffer );
    }

    return buffer;
}

} // namespace como

#endif // PACKABLE_LIST_HPP
