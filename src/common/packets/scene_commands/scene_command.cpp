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

SceneCommand::SceneCommand( SceneCommandType type, std::uint32_t userID ) :
    type_( type ),
    userID_( userID )
{
}


/***
 * 2. Packing and unpacking
 ***/


char* SceneCommand::packHeader( char* buffer ) const
{
    //( reinterpret_cast< SceneCommandType >( buffer ) )[0] = type_;
    //( reinterpret_cast< std::uint32_t >(buffer) )
}


const char* SceneCommand::unpacHeader( const char* buffer )
{

}


/***
 * 3. Getters
 ***/

std::uint16_t SceneCommand::getPacketSize() const
{
    return sizeof( type_ ) + sizeof( userID_ );
}

} // namespace como
