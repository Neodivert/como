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

#ifndef SCENE_COMMAND_HPP
#define SCENE_COMMAND_HPP

#include "../packable.hpp"

namespace como {

enum class SceneCommandType : std::uint8_t
{
    USER_CONNECTED = 0
};


class SceneCommand : public Packable
{
    private:
        SceneCommandType type_;
        std::uint32_t userID_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        SceneCommand( SceneCommandType type, std::uint32_t userID );


        /***
         * 2. Packing and unpacking
         ***/
        virtual void pack( char* buffer ) const = 0;
        virtual void unpack( const char* buffer ) = 0;
    private:
        char* packHeader( char* buffer ) const ;
        const char* unpacHeader( const char* buffer );
    public:


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const = 0;
};

} // namespace como

#endif // SCENE_COMMAND_HPP
