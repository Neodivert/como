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

#ifndef NEW_USER_PACKET_HPP
#define NEW_USER_PACKET_HPP

#include "packet.hpp"
#include <common/packables/packable_string.hpp>

namespace como {

/*!
 * \class NewUserPacket
 *
 * \brief Packet sent from user to server when the former tries to connect to
 * the latter.
 */
class NewUserPacket : public Packet
{  
    public:
        // Name of the user trying to connect to server
        PackableString name_;

    public:
        /***
         * 1. Construction
         ***/
        NewUserPacket();
        NewUserPacket( const char* name );
        NewUserPacket( const NewUserPacket& b );
        NewUserPacket( NewUserPacket&& ) = delete;
        virtual Packet* clone() const;


        /***
         * 2. Destruction
         ***/
        virtual ~NewUserPacket() = default;


        /***
         * 3. Getters
         ***/
        virtual bool expectedType() const;
        const char* getName() const ;


        /***
         * 4. Setters
         ***/
        void setName( const char* name );


        /***
         * 5. Operators
         ***/
        NewUserPacket& operator = ( const NewUserPacket& b );
        NewUserPacket& operator = ( NewUserPacket&& ) = delete;
};

} // namespace como

#endif // NEW_USER_PACKET_HPP
