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

#ifndef USER_ACCEPTANCE_PACKET_HPP
#define USER_ACCEPTANCE_PACKET_HPP

#include "packet.hpp"
#include <cstring>

#include "../packables/packables.hpp"
#include "../utilities/ids.hpp"

namespace como {

class UserAcceptancePacket : public Packet
{
    public:
        PackableUserID id_; 
        PackableString<NAME_SIZE> name_;
        PackableString<NAME_SIZE> sceneName_;
        PackableRGBA selectionColor_; // RGBA format.

    public:
        /***
         * 1. Construction
         ***/
        UserAcceptancePacket();
        UserAcceptancePacket( const std::uint32_t& id, const char* name, const char*sceneName, const std::uint8_t* selectionColor );
        UserAcceptancePacket( const UserAcceptancePacket& b );
        UserAcceptancePacket( UserAcceptancePacket&& ) = delete;
        virtual Packet* clone() const ;


        /***
         * 2. Destruction
         ***/
        ~UserAcceptancePacket() = default;


        /***
         * 3. Getters
         ***/
        std::uint32_t getId() const ;
        const char* getName() const ;
        const char* getSceneName() const ;
        const std::uint8_t* getSelectionColor() const ;
        virtual bool expectedType() const ;


        /***
         * 4. Setters
         ***/
        void setData( const std::uint32_t& id, const char* name, const std::uint8_t* selectionColor );
        void setId( const std::uint32_t& id );
        void setName( const char* name );
        void setSceneName( const char* name );
        void setSelectionColor( const std::uint8_t& r, const std::uint8_t& g, const std::uint8_t& b, const std::uint8_t& a );


        /***
         * 5. Operators
         ***/
        UserAcceptancePacket& operator = (const UserAcceptancePacket& b );
        UserAcceptancePacket& operator = ( UserAcceptancePacket&& ) = delete;
};

} // namespace como

#endif // USER_ACCEPTANCE_PACKET_HPP
