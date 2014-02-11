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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "user_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

UserCommand::UserCommand( UserCommandType userCommandType, UserID userID ) :
    Command( CommandTarget::USER, userID ),
    commandType_( userCommandType )
{
}


UserCommand::UserCommand( const UserCommand& b ) :
    Command( b ),
    commandType_( b.commandType_ )
{
}


/***
 * 2. Packing and unpacking
 ***/

char* UserCommand::pack( char* buffer ) const
{
    // Pack Command attributes.
    buffer = Command::pack( buffer );

    // Pack UserCommand attributes.
    packer::pack( static_cast< std::uint8_t >( commandType_ ), buffer );

    // Return the updated buffer.
    return buffer;
}


const char* UserCommand::unpack( const char* buffer )
{
    std::uint8_t commandType;

    // Unpack Command attributes.
    buffer = Command::unpack( buffer );

    // Unpack UserCommand attributes.
    packer::unpack( commandType, buffer );

    // TODO: Remove this check in future versions.
    if( static_cast< UserCommandType >( commandType ) == commandType_ ){
        throw std::runtime_error( "ERROR: Unexpected UserCommand" );
    }

    // Return the updated buffer.
    return buffer;
}


/***
 * 3. Getters
 ***/

UserCommandType UserCommand::getType() const
{
    return commandType_;
}


std::uint16_t UserCommand::getPacketSize() const
{
    return Command::getPacketSize() +
            sizeof( commandType_ );
}


/***
 * 4. Buffer pre reading
 ***/

UserCommandType UserCommand::getType( const char* buffer )
{
    return static_cast< const UserCommandType >( *( reinterpret_cast< const std::uint8_t* >( buffer ) ) );
}


} // namespace como
