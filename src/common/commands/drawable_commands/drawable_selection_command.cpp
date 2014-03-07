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

#include "drawable_selection_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

DrawableSelectionCommand::DrawableSelectionCommand() :
    DrawableCommand( DrawableCommandType::DRAWABLE_SELECTION, 0, NULL_DRAWABLE_ID ),
    addToSelection_( false )
{
    addPackable( &addToSelection_ );
}

DrawableSelectionCommand::DrawableSelectionCommand( UserID userID, PackableDrawableID drawableID, bool addToSelection ) :
    DrawableCommand( DrawableCommandType::DRAWABLE_SELECTION, userID, drawableID ),
    addToSelection_( addToSelection )
{
    addPackable( &addToSelection_ );
}


DrawableSelectionCommand::DrawableSelectionCommand( const DrawableSelectionCommand& b ) :
    DrawableCommand( b ),
    addToSelection_( b.addToSelection_ )
{
    addPackable( &addToSelection_ );
}


/***
 * 3. Getters
 ***/


std::uint8_t DrawableSelectionCommand::getAddToSelection() const
{
    return addToSelection_.getValue();
}


/***
 * 4. Setters
 ***/

void DrawableSelectionCommand::setAddToSelection( std::uint8_t addToSelection )
{
    addToSelection_ = addToSelection;
}

} // namespace como
