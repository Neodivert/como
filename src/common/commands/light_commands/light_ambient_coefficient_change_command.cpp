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

#include "light_ambient_coefficient_change_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

LightAmbientCoefficientChangeCommand::LightAmbientCoefficientChangeCommand() :
    LightCommand( LightCommandType::LIGHT_AMBIENT_COEFFICIENT_CHANGE, NO_USER, NO_RESOURCE ),
    ambientCoefficient_( 0.0f )
{
    addPackable( &ambientCoefficient_ );
}


LightAmbientCoefficientChangeCommand::LightAmbientCoefficientChangeCommand( UserID userID, ResourceID lightID, float ambientCoefficient ) :
    LightCommand( LightCommandType::LIGHT_AMBIENT_COEFFICIENT_CHANGE, userID, lightID ),
    ambientCoefficient_( ambientCoefficient )
{
    addPackable( &ambientCoefficient_ );
}


/***
 * 3. Getters
 ***/

float LightAmbientCoefficientChangeCommand::getAmbientCoefficient() const
{
    return ambientCoefficient_.getValue();
}



} // namespace como
