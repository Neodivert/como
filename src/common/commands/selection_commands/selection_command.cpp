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

#include "selection_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

SelectionCommand::SelectionCommand( SelectionCommandType selectionCommandType, UserID userID ) :
    Command( CommandTarget::SELECTION, userID ),
    commandType_( selectionCommandType )
{
    addBodyPackable( &commandType_ );
}


SelectionCommand::SelectionCommand( const SelectionCommand& b ) :
    Command( b ),
    commandType_( b.commandType_ )
{
    addBodyPackable( &commandType_ );
}


/***
 * 3. Getters
 ***/

SelectionCommandType SelectionCommand::getType() const
{
    return commandType_.getValue();
}


/***
 * 4. Buffer pre reading
 ***/

SelectionCommandType SelectionCommand::getType( const void* buffer )
{
    return *( static_cast< const SelectionCommandType* >( buffer ) );
}


} // namespace como
