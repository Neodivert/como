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

#include "command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

Command::Command( CommandTarget commandTarget, UserID userID ) :
    commandTarget_( commandTarget ),
    userID_( userID )
{
}


Command::Command( const Command& b ) :
    Packable( b ),
    commandTarget_( b.commandTarget_ ),
    userID_( b.userID_ )
{
}


/***
 * 3. Packing and unpacking
 ***/

char* Command::pack( char* buffer ) const
{
    // Pack the Command's "header".
    packer::pack( static_cast< std::uint8_t >( commandTarget_ ), buffer );
    packer::pack( userID_, buffer );

    // Return the updated buffer.
    return buffer;
}


const char* Command::unpack( const char* buffer )
{
    std::uint8_t unpackedCommandTarget;

    // Unpack the Command's "header".
    packer::unpack( unpackedCommandTarget, buffer );
    // TODO: Modify SCENE_UPDATE structure so this check isn't necesary.
    if( static_cast< CommandTarget >( unpackedCommandTarget ) != commandTarget_ ){
        throw std::runtime_error( "Unexpected packet when unpacking" );
    }
    packer::unpack( userID_, buffer );

    // Return the updated buffer.
    return buffer;
}


/***
 * 4. Getters
 ***/

CommandTarget Command::getTarget() const
{
    return commandTarget_;
}


std::uint16_t Command::getPacketSize() const
{
    return sizeof( CommandTarget ) + sizeof( userID_ );
}


UserID Command::getUserID() const
{
    return userID_;
}


/***
 * 5. Buffer pre reading
 ***/

CommandTarget Command::getTarget( const char* buffer )
{
    return static_cast< const CommandTarget >(*buffer);
}


/***
 * 6. Setters
 ***/

void Command::setUserID( const UserID& userID )
{
    userID_ = userID;
}

} // namespace como
