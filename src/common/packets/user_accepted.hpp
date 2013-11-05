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

#include "packet.hpp"
#include <cstring>

namespace como {

class UserAccepted : public Packet
{
    public:
        ID id_;
        char name_[NAME_SIZE];
        std::uint8_t selectionColor_[4]; // RGBA format.

    public:
        /***
         * 1. Initialization and destruction
         ***/
        UserAccepted();
        UserAccepted( const std::uint32_t& id, const char* name, const std::uint8_t* selectionColor );
        void setData( const std::uint32_t& id, const char* name, const std::uint8_t* selectionColor );
        UserAccepted( const UserAccepted& b );
        virtual Packet* clone() const ;


        /***
         * 2. Packing and unpacking
         ***/
        virtual char* packBody( char* buffer ) const ;
        virtual const char* unpackBody( const char* buffer );


        /***
         * 3. Getters
         ***/
        std::uint32_t getId() const ;
        const char* getName() const ;
        const std::uint8_t* getSelectionColor() const ;
        virtual bool expectedType() const ;


        /***
         * 4. Setters
         ***/
        void setId( const std::uint32_t& id );
        void setName( const char* name );
        void setSelectionColor( const std::uint8_t& r, const std::uint8_t& g, const std::uint8_t& b, const std::uint8_t& a );
};

} // namespace como

#endif // USER_ACCEPTED_HPP
