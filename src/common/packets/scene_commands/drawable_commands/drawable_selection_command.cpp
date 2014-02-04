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

#include "drawable_selection_command.hpp"

namespace como {

/***
 * 1. Initialization and destruction.
 ***/

DrawableSelectionCommand::DrawableSelectionCommand() :
    DrawableCommand( SceneCommandType::DRAWABLE_SELECTION ),
    addToSelection_( false )
{
}


DrawableSelectionCommand::DrawableSelectionCommand( UserID userID, DrawableID drawableID, bool addToSelection ) :
    DrawableCommand( userID, drawableID, SceneCommandType::DRAWABLE_SELECTION ),
    addToSelection_( addToSelection == true )
{
}


DrawableSelectionCommand::DrawableSelectionCommand( const DrawableSelectionCommand& b ) :
    DrawableCommand( b ),
    addToSelection_( b.addToSelection_ )
{
}


/***
 * 2. Packing and unpacking
 ***/

char* DrawableSelectionCommand::pack( char* buffer ) const
{
    // Pack the Drawable command fields.
    buffer = DrawableCommand::pack( buffer );

    // Pack the "add to selection" field.
    packer::pack( addToSelection_, buffer );

    // Return the buffer updated pointer.
    return buffer;
}


const char* DrawableSelectionCommand::unpack( const char* buffer )
{
    // Unpack the Drawable command fields.
    buffer = DrawableCommand::unpack( buffer );

    // Unpack the "add to selection" field.
    packer::unpack( addToSelection_, buffer );

    // Return the buffer updated pointer.
    return buffer;
}


/***
 * 3. Getters
 ***/

std::uint16_t DrawableSelectionCommand::getPacketSize() const
{
    return DrawableCommand::getPacketSize() +
            sizeof( addToSelection_ );
}


std::uint8_t DrawableSelectionCommand::getAddToSelection() const
{
    return addToSelection_;
}


/***
 * 4. Setters
 ***/

void DrawableSelectionCommand::setAddToSelection( std::uint8_t addToSelection )
{
    addToSelection_ = addToSelection;
}

} // namespace como
