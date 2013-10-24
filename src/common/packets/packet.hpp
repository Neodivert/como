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

#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include "packable.hpp"
#include "../utilities/endianness.hpp"
#include <stdexcept>

namespace como {

enum class PacketType : std::int8_t
{
    NEW_USER = 0,
    USER_ACCEPTED = 1,
    SCENE_UPDATE = 2
};

class Packet : public Packable
{
    private:
        PacketType type_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Packet( PacketType type );


        /***
         * 2. Packing and unpacking
         ***/
        virtual void pack( char* buffer ) const = 0;
        virtual void unpack( const char* buffer ) = 0;
    protected:
        char* packHeader( char* buffer ) const ;
        const char* unpackHeader( const char* buffer );
    public:


        /***
         * 3. Getters
         ***/
        PacketType getType() const ;
        //static std::uint16_t getType( const char* buffer ) ;
        virtual std::uint16_t getPacketSize() const ;
};

} // namespace como

#endif // PACKAGE_HPP
