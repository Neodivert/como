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

#include "light_color_change_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

LightColorChangeCommand::LightColorChangeCommand() :
    LightCommand( LightCommandType::LIGHT_COLOR_CHANGE, NO_USER, NO_RESOURCE )
{
    addPackable( &lightColor_ );
}


LightColorChangeCommand::LightColorChangeCommand( UserID userID, ResourceID lightID, Color lightColor ) :
    LightCommand( LightCommandType::LIGHT_COLOR_CHANGE, userID, lightID ),
    lightColor_( lightColor )
{
    addPackable( &lightColor_ );
}


LightColorChangeCommand::LightColorChangeCommand( const LightColorChangeCommand& b ) :
    LightCommand( b ),
    lightColor_( b.lightColor_ )
{
    addPackable( &lightColor_ );
}


/***
 * 3. Getters
 ***/

Color LightColorChangeCommand::getLightColor() const
{
    return lightColor_.getValue();
}

} // namespace como
