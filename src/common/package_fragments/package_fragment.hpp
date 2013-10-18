/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
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

#ifndef PACKAGE_FRAGMENT_HPP
#define PACKAGE_FRAGMENT_HPP

#include "../utilities/endianness.hpp"

namespace como {

const unsigned int NAME_SIZE = 64;

class PackageFragment
{
    public:
        /***
         * 1. Encoding and decoding
         ***/
        virtual void encode( char* code ) const = 0;
        virtual void decode( const char* code ) = 0;


        /***
         * 2. Getters
         ***/
        virtual std::uint16_t getCodeSize() = 0;
};

} // namespace como

#endif // PACKAGE_FRAGMENT_HPP
