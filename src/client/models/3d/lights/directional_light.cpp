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

#include "directional_light.hpp"

namespace como {

/***
 * 1. Construction
 ***/

DirectionalLight::DirectionalLight( glm::vec3 position, glm::vec3 lightColor ) :
    Light( "data/primitives/system/camera.obj", 0, position, lightColor )
{}


/***
 * 3. Setters
 ***/

void DirectionalLight::setPosition( const glm::vec3& position )
{
    Q_UNUSED( position );
}

} // namespace como
