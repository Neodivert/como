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

#ifndef PACKET_HEADER_HPP
#define PACKET_HEADER_HPP

#include "../packables/packables.hpp"

namespace como {

enum class PacketType : std::uint8_t
{
    NEW_USER = 0,
    USER_ACCEPTED = 1,
    SCENE_UPDATE = 2
};

class PacketHeader : public CompositePackable
{
    private:
        PackableUint8< PacketType > type_;
        PackableUint16< std::uint16_t > bodySize_;

    public:
        /***
         * 1. Construction
         ***/
        PacketHeader( PacketType type );
        PacketHeader( const PacketHeader& );
        PacketHeader( PacketHeader&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~PacketHeader(){}


        /***
         * 3. Getters
         ***/
        PacketType getType() const;
        std::uint16_t getBodySize() const;


        /***
         * 4. Setters
         ***/
        void setBodySize( std::uint16_t bodySize );


        /***
         * 5. Operators
         ***/
        CompositePackable& operator = (const PacketHeader& b);
        CompositePackable& operator = ( PacketHeader&& );
};

} // namespace como

#endif // PACKET_HEADER_HPP
