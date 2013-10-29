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

#include "scene_command.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

SceneCommand::SceneCommand( SceneCommandType type ) :
    type_( type ),
    userID_( 0 )
{
}


/***
 * 2. Packing and unpacking
 ***/

char* SceneCommand::pack( char* buffer ) const
{
    // Pack the SceneCommand's "header".
    packer::pack( static_cast< std::uint8_t >( type_ ), buffer );
    packer::pack( userID_, buffer );

    // Return the updated buffer.
    return buffer;
}


const char* SceneCommand::unpack( const char* buffer )
{
    std::uint8_t type;

    // Pack the SceneCommand's "header".
    packer::unpack( type, buffer );
    type_ = static_cast< SceneCommandType >( type );
    packer::unpack( userID_, buffer );

    // Return the updated buffer.
    return buffer;
}


/***
 * 3. Getters
 ***/

std::uint16_t SceneCommand::getPacketSize() const
{
    return sizeof( type_ ) + sizeof( userID_ );
}


SceneCommandType SceneCommand::getType() const
{
    return type_;
}


SceneCommandType SceneCommand::getType( const char* buffer )
{
    return *( reinterpret_cast< const SceneCommandType* >( buffer ) );
}


ID SceneCommand::getUserID() const
{
    return userID_;
}


/***
 * 4. Setters
 ***/

void SceneCommand::setUserID( const ID& userID )
{
    userID_ = userID;
}

} // namespace como
