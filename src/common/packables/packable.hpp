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

#ifndef PACKABLE_HPP
#define PACKABLE_HPP

#include <cstdint>
#include <stdexcept>
#include <iostream>

namespace como {

const unsigned int NAME_SIZE = 64;

/**
 * \class Packable
 *
 * \brief Interface for all entities which can be packed / unpacked and sent
 * through the network.
 */

class Packable
{   
    public:
        /***
         * 1. Construction
         ***/
        /// Creates an emtpy packable.
        Packable() = default;

        /// Creates a copy from another packable.
        Packable( const Packable& ) = default;

        /// Creates a "move copy" from another packable.
        Packable( Packable&& ) = default;


        /***
         * 2. Destruction
         ***/
        /// Destroys the packable.
        virtual ~Packable(){}


        /***
         * 3. Packing and unpacking
         ***/

        /** \brief Packs this packable into the given buffer.
        * \param buffer a pointer to the buffer where this packable will be packed into.
        * \return a pointer to the first position in the buffer after this packable's packed data.
        */
        virtual void* pack( void* buffer ) const = 0;

        /** \brief Unpacks this packable from the given buffer.
        * \param buffer a pointer to the buffer where this packable will be unpacked from.
        * \return a pointer to the first position in the buffer after this packable's packed data.
        */
        virtual const void* unpack( const void* buffer ) = 0;

        /** \brief Unpacks data from the given buffer AND THROW AN EXCEPTION if it is different from
        * this packable's inner data.
        * \param buffer a pointer to the buffer where the data will be unpacked from.
        * \return a pointer to the first position in the buffer after this packable's packed data.
        */
        virtual const void* unpack( const void* buffer ) const = 0;


        /***
         * 3. Getters
         ***/

        /** \brief Returns the size that this packable's data would occupy once packed.
        * \return the size that this packable's data would occupy once packed.
        */
        virtual std::uint16_t getPacketSize() const = 0;


        /***
         * 5. Operators
         ***/
        // TODO: Make it possible to assign packables.
        Packable& operator = (const Packable& b) = delete;
        Packable& operator = ( Packable&& ) = delete;
};

} // namespace como

#endif // PACKABLE_HPP
