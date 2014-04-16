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

#include "lighting_manager.hpp"

namespace como {

LightingManager::LightingManager() :
    directionalLight_( new DirectionalLight( glm::vec3( 1.0f, 1.0f, 1.0f ) ) )
{}


/***
 * 3. Getters
 ***/

DirectionalLightPtr LightingManager::getDirectionalLight()
{
    return directionalLight_;
}

} // namespace como
