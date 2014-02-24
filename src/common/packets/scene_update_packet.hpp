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

#ifndef SCENE_UPDATE_PACKET_HPP
#define SCENE_UPDATE_PACKET_HPP

#include "packet.hpp"
#include "../commands/commands.hpp"
#include <vector>
#include <list>

namespace como {

typedef std::list< CommandConstPtr > CommandsList;

class SceneUpdatePacket : public Packet
{
    private:
        std::uint32_t nUnsyncCommands_;
        std::vector< CommandConstPtr > commands_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        SceneUpdatePacket();
        SceneUpdatePacket( const SceneUpdatePacket& b );
        SceneUpdatePacket( SceneUpdatePacket&& ) = delete;
        virtual Packet* clone() const ;

        ~SceneUpdatePacket() = default;


        /***
         * 2. Packing and unpacking
         ***/
        virtual char* packBody( char* buffer ) const ;
        virtual const char* unpackBody( const char* buffer );


        /***
         * 3. Getters
         ***/
        std::uint32_t getUnsyncCommands() const ;
        const std::vector< CommandConstPtr >* getCommands() const ;
        virtual bool expectedType() const ;


        /***
         * 4. Setters
         ***/
        void addCommand( CommandConstPtr command );
        void addCommand( CommandConstPtr command,
                         const std::uint32_t& commandIndex,
                         const std::uint32_t& historicSize );
        /*
        void addCommands( const CommandsList* commandsHistoric,
                          const std::uint32_t& firstCommand,
                          const std::uint8_t& maxCommands );*/
        void clear();


        /***
         * 5. Operators
         ***/
        SceneUpdatePacket& operator = (const SceneUpdatePacket& b) = delete;
        SceneUpdatePacket& operator = ( SceneUpdatePacket&& ) = delete;
};

typedef std::shared_ptr< SceneUpdatePacket > SceneUpdatePacketPtr;
typedef std::shared_ptr< const SceneUpdatePacket > SceneUpdatePacketConstPtr;

} // namespace como

#endif // SCENE_UPDATE_PACKET_HPP
