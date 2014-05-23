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

#include "abstract_selection_parameter_change_command.hpp"

namespace como {


/***
 * 1. Construction
 ***/

AbstractSelectionParameterChangeCommand::AbstractSelectionParameterChangeCommand( SelectionParameterName parameterName ) :
    SelectionCommand( SelectionCommandType::PARAMETER_CHANGE, NO_USER ),
    parameterName_( parameterName )
{
    addPackable( &parameterName_ );
}


AbstractSelectionParameterChangeCommand::AbstractSelectionParameterChangeCommand( SelectionParameterName parameterName, UserID userID ) :
    SelectionCommand( SelectionCommandType::PARAMETER_CHANGE, userID ),
    parameterName_( parameterName )
{
    addPackable( &parameterName_ );
}


AbstractSelectionParameterChangeCommand::AbstractSelectionParameterChangeCommand( const AbstractSelectionParameterChangeCommand& b ) :
    SelectionCommand( b ),
    parameterName_( b.parameterName_ )
{
    addPackable( &parameterName_ );
}


/***
 * 3. Getters
 ***/

SelectionParameterName AbstractSelectionParameterChangeCommand::getParameterName() const
{
    return parameterName_.getValue();
}


/***
 * 4. Buffer pre-reading
 ***/

SelectionParameterName AbstractSelectionParameterChangeCommand::getParameterName( const void* buffer )
{
    SelectionCommand selectionCommand( SelectionCommandType::SELECTION_RESPONSE, NO_USER );

    return static_cast< SelectionParameterName>( (static_cast< const std::uint8_t* >( buffer ))[selectionCommand.getPacketSize()] );
}

} // namespace como
