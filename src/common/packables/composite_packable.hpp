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

#ifndef COMPOSITE_PACKABLE_HPP
#define COMPOSITE_PACKABLE_HPP

#include "packable.hpp"
#include <vector>

namespace como {

class CompositePackable : public Packable
{
    private:
        // Yes, simple pointers. We are NOT owning the pointed packables.
        std::vector< Packable* > headerPackables_;
        std::vector< Packable* > bodyPackables_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        CompositePackable() = default;
        CompositePackable( const CompositePackable& ) = delete; // We can't copy pointers to packables we don't own.
        CompositePackable( CompositePackable&& ) = delete; // We can't copy pointers to packables we don't own.

        virtual ~CompositePackable(){}


        /***
         * 2. Packing and unpacking
         ***/
        virtual void* pack( void* buffer ) const;
        virtual const void* unpack( const void* buffer );
        virtual void* packHeader( void* buffer ) const;
        virtual const void* unpackHeader( const void* buffer );
        virtual void* packBody( void* buffer ) const;
        virtual const void* unpackBody( const void* buffer );


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const;
        virtual std::uint16_t getPacketHeaderSize() const;
        virtual std::uint16_t getPacketBodySize() const;


        /***
         * 4. Packables management
         ***/
    protected:
        void addHeaderPackable( Packable* packable );
        void addBodyPackable( Packable* packable );
    public:


        /***
         * 5. Operators
         ***/
        CompositePackable& operator = (const CompositePackable& b) = delete;
        CompositePackable& operator = ( CompositePackable&& ) = delete;
};

} // namespace como

#endif // COMPOSITE_PACKABLE_HPP
