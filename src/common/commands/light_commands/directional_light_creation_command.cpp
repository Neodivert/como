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

#include "directional_light_creation_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

DirectionalLightCreationCommand::DirectionalLightCreationCommand() :
    LightCreationCommand( LightType::DIRECTIONAL_LIGHT, NO_USER, NULL_DRAWABLE_ID, PackableColor() ) // TODO: Remove MaterialID() and PackableColor()
{}


DirectionalLightCreationCommand::DirectionalLightCreationCommand( UserID userID, LightID lightID, const PackableColor& lightColor ) :
    LightCreationCommand( LightType::DIRECTIONAL_LIGHT, userID, lightID, lightColor )
{}

} // namespace como
