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

#include "selection_response_command.hpp"
#include <stdexcept>

namespace como {

/***
 * 1. Initialization and destruction
 ***/

SelectionResponseCommand::SelectionResponseCommand() :
    SelectionCommand( SelectionCommandType::SELECTION_RESPONSE, 0 ), // UserID: 0 - Server
    nSelections_( 0 ),
    selectionConfirmed_( 0 )
{
    addPackable( &nSelections_ );
    addPackable( &selectionConfirmed_ );
}


SelectionResponseCommand::SelectionResponseCommand( const SelectionResponseCommand& b ) :
    SelectionCommand( b ),
    nSelections_( b.nSelections_ ),
    selectionConfirmed_( b.selectionConfirmed_ )
{
    addPackable( &nSelections_ );
    addPackable( &selectionConfirmed_ );
}


/***
 * 3. Getters
 ***/

std::uint8_t SelectionResponseCommand::getNSelections() const
{
    return nSelections_.getValue();
}


std::uint32_t SelectionResponseCommand::getSelectionConfirmed() const
{
    return selectionConfirmed_.getValue();
}


/***
 * 4. Setters
 ***/

void SelectionResponseCommand::addSelectionConfirmation( bool confirmed )
{
    std::uint8_t confirmationFlag = confirmed ? 1 : 0;

    if( nSelections_.getValue() >= MAX_SELECTION_CONFIRMATIONS ){
        throw std::runtime_error( "Selection confirmations exceeds the limit" );
    }

    // Add the confirmation flag to the bits flag.
    confirmationFlag <<= nSelections_.getValue();
    selectionConfirmed_ = selectionConfirmed_.getValue() | confirmationFlag;

    nSelections_ = nSelections_.getValue() + 1;
}


void SelectionResponseCommand::clear()
{
    nSelections_ = 0;
    selectionConfirmed_ = 0;
}

} // namespace como
