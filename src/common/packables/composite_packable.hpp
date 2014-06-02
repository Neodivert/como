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

#ifndef COMPOSITE_PACKABLE_HPP
#define COMPOSITE_PACKABLE_HPP

#include "packable.hpp"
#include <vector>

namespace como {

/*!
 * \class PackablePair
 *
 * \brief Pair consisting of a constant and a variable pointers, both pointing
 * to the same object. This auxiliar class was made for making CompositePackable
 * capable of holding both constant and variable packables.
 */
// TODO: Move to a separate file. Check if Doxygen recognizes it then.

struct PackablePair {
    // Yes, simple pointers. We are NOT owning the pointed packables.
    const Packable* constant;
    Packable* variable;


    PackablePair() :
        constant( nullptr ),
        variable( nullptr )
    {}

    PackablePair( const Packable* packable ) :
        constant( packable ),
        variable( nullptr )
    {}

    PackablePair( Packable* packable ) :
        constant( packable ),
        variable( packable )
    {}

    PackablePair( const PackablePair& b ) :
        constant( b.constant ),
        variable( b.variable )
    {}
};


/*!
 * \class CompositePackable
 *
 * \brief Packable container which holds a list of PackablePair
 * elements. Packing / unpacking a CompositePackable means packing /
 * unpacking all its contained packable elements in the same order in which they
 * were added to the container.
 *
 * IMPORTANT: The packables pointed inside this container AREN'T destroyer
 * within its destructor, leaving this responsability to the user of this class.
 */

class CompositePackable : public Packable
{
    private:
        /*!
         * Vector of *pointers* to Packables held by this container. The
         * pointed objects AREN'T destroyed within this class destructor,
         * leaving this responsability to this class' user.
        */
        std::vector< PackablePair > packables_;

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor. */
        CompositePackable() = default;

        /*!
         * \brief Copy constructor.
         *
         * This copy constructor DOESN'T copy the packables held by source
         * CompositePackable because we don't own them.
        */
        CompositePackable( const CompositePackable& b ) : Packable( b ){} // We can't copy pointers to packables we don't own.

        /*! \brief Move constructor. */
        CompositePackable( CompositePackable&& ) = delete; // We can't copy pointers to packables we don't own.


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor. */
        virtual ~CompositePackable(){}


        /***
         * 3. Packing and unpacking
         ***/

        /*!
         * \brief Packs all the packables held by this class in the same order
         * in which they were inserted.
         * \param buffer a pointer to the buffer the data are packed into.
         * \return a pointer to the next available position in the given buffer.
         */
        virtual void* pack( void* buffer ) const;

        /*!
         * \brief Unpacks all the packables held by this class in the same order
         * in which they were inserted.
         * \param buffer a pointer to the buffer the data are unpacked from.
         * \return a pointer to the next position in the given buffer.
         */
        virtual const void* unpack( const void* buffer );

        /*!
         * \brief Testing unpacking - Analogous to unpack(), but this one
         * throws an exception if the unpacked data doesn't match the object's
         * data.
         * \param buffer a pointer to the buffer the data are unpacked from.
         * \return a pointer to the next position in the given buffer.
         */
        virtual const void* unpack( const void* buffer ) const ;


        /***
         * 4. Getters
         ***/

        /*!
         * \brief returns the size (in bytes) that the data held by this
         * container would ocuppy once packed.
         * \return an integer with the size (in bytes) that this object
         * would ocuppy once packed.
         */
        virtual PacketSize getPacketSize() const;
    protected:

        /*!
         * \brief returns the number of packables held by this container.
         * \return an integer indicating the number of packables held by this
         * container.
         */
        std::uint8_t getNumberOfPackables() const;


        /*!
         * \brief returns the offset (in bytes) of the packable given as an
         * argument.
         */
        unsigned int getOffset( Packable* packable ) const;

    public:


        /***
         * 5. Packables management
         ***/
    protected:

        /*!
         * \brief adds a mutable packable to this container.
         * \param packable to be added to the container.
         */
        void addPackable( Packable* packable );

        /*!
         * \brief adds a constant packable to this container.
         * \param packable packable to be added to the container.
         */
        void addPackable( const Packable* packable );

        /*!
         * \brief removes nElements packables from this container including
         * firstPackable.
         * \param firstPackable the first packable to be removed from
         * this container
         * \param nElements Number of elements to be removed.
         */
        void removePackables( const Packable* firstPackable, unsigned int nElements );
    public:


        /***
         * 6. Operators
         ***/

        /*! \brief Copy assignment operator */
        CompositePackable& operator = (const CompositePackable& b) = delete;

        /*! \brief Move assignment operator */
        CompositePackable& operator = ( CompositePackable&& ) = delete;
};

} // namespace como

#endif // COMPOSITE_PACKABLE_HPP
