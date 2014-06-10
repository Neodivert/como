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
    LightCommand( LightCommandType::LIGHT_COLOR_CHANGE, NULL_DRAWABLE_ID )
{
    addPackable( &lightColor_ );
}


LightColorChangeCommand::LightColorChangeCommand( PackableLightID lightID, PackableColor lightColor ) :
    LightCommand( LightCommandType::LIGHT_COLOR_CHANGE, lightID ),
    lightColor_( lightColor )
{
    addPackable( &lightColor_ );
}


/***
 * 3. Getters
 ***/

PackableColor LightColorChangeCommand::getLightColor() const
{
    return lightColor_;
}

} // namespace como
