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
#include <memory>

namespace como {

enum class SceneCommandType : std::uint8_t
{
    USER_CONNECTED = 0,
    USER_DISCONNECTED = 1
};


class SceneCommand : public Packable
{
    private:
        SceneCommandType type_;
        UserID userID_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        SceneCommand( SceneCommandType type );
        SceneCommand( SceneCommandType type, UserID userID );
        SceneCommand( const SceneCommand& b );

        virtual ~SceneCommand(){}


        /***
         * 2. Packing and unpacking
         ***/
        virtual char* pack( char* buffer ) const ;
        virtual const char* unpack( const char* buffer ) ;


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const ;
        SceneCommandType getType() const ;
        static SceneCommandType getType( const char* buffer );
        UserID getUserID() const ;


        /***
         * 4. Setters
         ***/
        void setUserID( const UserID& userID );
};

typedef std::shared_ptr< SceneCommand > SceneCommandPtr;
typedef std::shared_ptr< const SceneCommand > SceneCommandConstPtr;

} // namespace como

#endif // SCENE_COMMAND_HPP
