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

#ifndef SCENE_UPDATE_HPP
#define SCENE_UPDATE_HPP

#include "packet.hpp"

namespace como {

class SceneUpdate : public Packet
{
    private:
        std::uint32_t lastCommandSent_;
        std::uint32_t lastCommandConfirmed_;
        std::uint32_t nUnsyncCommands;
        //std::vector< SceneCommand > sceneCommands;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        SceneUpdate( PacketType type );


        /***
         * 2. Packing and unpacking
         ***/
        virtual void pack( char* buffer ) const = 0;
        virtual void unpack( const char* buffer ) = 0;


        /***
         * 3. Getters
         ***/
        //static std::uint16_t getType( const char* buffer ) ;
        //virtual std::uint16_t getPacketSize() const ;
};

} // namespace como

#endif // SCENE_UPDATE_HPP
