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

#include "directional_lamp.hpp"

namespace como {

/***
 * 1. Construction
 ***/

DirectionalLamp::DirectionalLamp( glm::vec3 position, glm::vec3 lightColor ) :
    Lamp( position, lightColor )
{}


/***
 * 3. Setters
 ***/

void DirectionalLamp::setPosition( const glm::vec3& position )
{
    Q_UNUSED( position );
}

} // namespace como
