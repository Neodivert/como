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

#ifndef USER_ACCEPTED_HPP
#define USER_ACCEPTED_HPP

#include "package_fragment.hpp"
#include <cstring>

namespace como {

class UserAccepted : public PackageFragment
{
    public:
        std::uint32_t id;
        char name[NAME_SIZE];
        std::uint8_t selectionColor[4]; // RGBA format.

    public:
        /***
         * 1. Packing and unpacking
         ***/
        virtual void pack( char* code ) const;
        virtual void unpack( const char* code );

        /***
         * 2. Getters
         ***/
        virtual std::uint16_t getCodeSize();
};

} // namespace como

#endif // USER_ACCEPTED_HPP
