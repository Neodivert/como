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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef PACKABLE_HPP
#define PACKABLE_HPP

#include "../utilities/packer.hpp"
#include "../utilities/ids.hpp"

namespace como {

const unsigned int NAME_SIZE = 64;

class Packable
{
    public:
        /***
         * 1. Initialization and destruction
         ***/
        Packable() = default;
        Packable( const Packable& ) = default;
        Packable( Packable&& ) = default;

        virtual ~Packable(){}


        /***
         * 2. Packing and unpacking
         ***/
        virtual char* pack( char* buffer ) const = 0;
        virtual const char* unpack( const char* buffer ) = 0;


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const = 0;


        /***
         * 5. Operators
         ***/
        Packable& operator = (const Packable& b) = delete;
        Packable& operator = ( Packable&& ) = delete;
};

} // namespace como

#endif // PACKABLE_HPP
