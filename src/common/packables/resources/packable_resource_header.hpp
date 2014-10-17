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

#ifndef PACKABLE_RESOURCE_HEADER_HPP
#define PACKABLE_RESOURCE_HEADER_HPP

#include <common/packables/abstract_packable_wrapper.hpp>
#include <common/packables/ids/packable_resource_id.hpp>
#include <common/packables/packable_string.hpp>
#include <common/resources/resource_header.hpp>

namespace como {

class PackableResourceHeader : public AbstractPackableWrapper< ResourceHeader >, public CompositePackable
{
    public:
        /***
         * 1. Construction
         ***/
        PackableResourceHeader();
        PackableResourceHeader( const ResourceHeader& );
        PackableResourceHeader( const PackableResourceHeader& );
        PackableResourceHeader( PackableResourceHeader&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~PackableResourceHeader() = default;


        /***
         * 3. Getters
         ***/
        virtual ResourceHeader getValue() const;
        virtual PacketSize getPacketSize() const;


        /***
         * 4. Setters
         ***/
        virtual void setValue( const ResourceHeader &header );


        /***
         * 5. Packing / unpacking
         ***/
        virtual void *pack( void *buffer ) const;
        virtual const void* unpack( const void* buffer );
        virtual const void* unpack( const void* buffer ) const;

        /***
         * 6. Operators
         ***/
        PackableResourceHeader& operator = ( const PackableResourceHeader& ) = delete;
        PackableResourceHeader& operator = ( PackableResourceHeader&& ) = delete;


    private:
        PackableResourceID id_;
        PackableString name_;
};

} // namespace como

#endif // PACKABLE_RESOURCE_HEADER_HPP
