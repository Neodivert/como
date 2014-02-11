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

#include "selection_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

SelectionCommand::SelectionCommand( SelectionCommandType selectionCommandType, UserID userID ) :
    Command( CommandTarget::SELECTION, userID ),
    commandType_( selectionCommandType )
{
}


SelectionCommand::SelectionCommand( const SelectionCommand& b ) :
    Command( b ),
    commandType_( b.commandType_ )
{
}


/***
 * 2. Packing and unpacking
 ***/

char* SelectionCommand::pack( char* buffer ) const
{
    // Pack Command attributes.
    buffer = Command::pack( buffer );

    // Pack SelectionCommand attributes.
    packer::pack( static_cast< std::uint8_t >( commandType_ ), buffer );

    // Return updated buffer pointer.
    return buffer;
}


const char* SelectionCommand::unpack( const char* buffer )
{
    std::uint8_t commandType;

    // Unpack Command attributes.
    buffer = Command::unpack( buffer );

    // Unpack SelectionCommand attributes.
    packer::unpack( commandType, buffer );

    // TODO: Remove this check in future versions.
    if( commandType_ != static_cast< SelectionCommandType >( commandType ) ){
        throw std::runtime_error( "ERROR: Unexpected SelectionCommand" );
    }

    // Return updated buffer pointer.
    return buffer;
}


/***
 * 3. Getters
 ***/

SelectionCommandType SelectionCommand::getType() const
{
    return commandType_;
}


std::uint16_t SelectionCommand::getPacketSize() const
{
    return Command::getPacketSize() +
            sizeof( commandType_ );
}


/***
 * 4. Buffer pre reading
 ***/

SelectionCommandType SelectionCommand::getType( const char* buffer )
{
    return static_cast< SelectionCommandType >( *( reinterpret_cast< const std::uint8_t* >( buffer ) ) );
}

} // namespace como
