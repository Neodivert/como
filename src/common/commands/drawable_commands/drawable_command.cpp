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

#include "drawable_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

DrawableCommand::DrawableCommand( DrawableCommandType drawableCommandType, UserID userID, PackableDrawableID drawableID ) :
    Command( CommandTarget::DRAWABLE, userID ),
    commandType_( drawableCommandType ),
    drawableID_( drawableID )
{
}


DrawableCommand::DrawableCommand( const DrawableCommand& b ) :
    Command( b ),
    commandType_( b.commandType_ ),
    drawableID_( b.drawableID_ )
{
}


/***
 * 3. Getters
 ***/

DrawableCommandType DrawableCommand::getType() const
{
    return commandType_.getValue();
}


PackableDrawableID DrawableCommand::getDrawableID() const
{
    return drawableID_;
}


/***
 * 5. Buffer pre reading
 ***/

DrawableCommandType DrawableCommand::getType( const void* buffer )
{
    return *( static_cast< const DrawableCommandType* >( buffer ) );
}


/***
 * 6. Setters
 ***/

void DrawableCommand::setDrawableID( const PackableDrawableID& drawableID )
{
    drawableID_ = drawableID;
}


} // namespace como
