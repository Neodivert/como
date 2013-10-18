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

#ifndef NEW_USER_HPP
#define NEW_USER_HPP

#include "package_fragment.hpp"

namespace como {

class NewUser : public PackageFragment
{  
    public:
        char name[NAME_SIZE];

    public:
        /***
         * 1. Encoding and decoding
         ***/
        virtual void encode( char* code ) const;
        virtual void decode( const char* code );


        /***
         * 2. Getters
         ***/
        virtual std::uint16_t getCodeSize();
};

} // namespace como

#endif // NEW_USER_HPP
