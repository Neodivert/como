/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published by
 * the Free Software Foundation.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "drawable_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

DrawableCommand::DrawableCommand( DrawableCommandType drawableCommandType, UserID userID, PackableDrawableID drawableID ) :
    TypeCommand( CommandTarget::DRAWABLE, drawableCommandType, userID ),
    drawableID_( drawableID )
{
    addPackable( &drawableID_ );
}


DrawableCommand::DrawableCommand( const DrawableCommand& b ) :
    TypeCommand( b ),
    drawableID_( b.drawableID_ )
{
    addPackable( &drawableID_ );
}


/***
 * 3. Getters
 ***/

PackableDrawableID DrawableCommand::getDrawableID() const
{
    return drawableID_;
}


/***
 * 4. Setters
 ***/

void DrawableCommand::setDrawableID( const PackableDrawableID& drawableID )
{
    drawableID_ = drawableID;
}


} // namespace como
