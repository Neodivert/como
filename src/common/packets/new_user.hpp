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

#include "packet.hpp"

namespace como {

class NewUser : public Packet
{  
    public:
        char name_[NAME_SIZE];

    public:
        /***
         * 1. Initialization and destruction
         ***/
        NewUser();
        NewUser( const char* name );


        /***
         * 2. Packing and unpacking
         ***/
        virtual char* packBody( char* buffer ) const ;
        virtual const char* unpackBody( const char* buffer );


        /***
         * 3. Getters
         ***/
        virtual bool expectedType() const;
        const char* getName() const ;


        /***
         * 4. Setters
         ***/
        void setName( const char* name );
};

} // namespace como

#endif // NEW_USER_HPP
