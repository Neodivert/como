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
#include "scene_commands/scene_commands.hpp"
#include <vector>
#include <list>

namespace como {

typedef std::list< SceneCommandConstPtr > CommandsList;

class SceneUpdate : public Packet
{
    private:
        std::uint32_t lastCommandSent_;
        std::uint32_t nUnsyncCommands_;
        std::vector< SceneCommandConstPtr > commands_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        SceneUpdate();


        /***
         * 2. Packing and unpacking
         ***/
        virtual char* pack( char* buffer ) const ;
        virtual const char* unpack( const char* buffer );


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const ;
        std::uint32_t getLasCommandSent() const ;
        std::uint32_t getUnsyncCommands() const ;
        const std::vector< SceneCommandConstPtr >* getCommands();


        /***
         * 4. Setters
         ***/
        void addCommands( const CommandsList* commandsHistoric,
                          const std::uint32_t& firstCommand,
                          const std::uint8_t& maxCommands );
};

} // namespace como

#endif // SCENE_UPDATE_HPP
