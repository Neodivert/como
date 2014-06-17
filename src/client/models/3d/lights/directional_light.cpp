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

#include "directional_light.hpp"

namespace como {

/***
 * 1. Construction
 ***/

DirectionalLight::DirectionalLight( DirectionalLightPropertiesSharedPtr lightProperties, MaterialConstPtr meshMaterial ) :
    Mesh( MeshType::LIGHT, "data/system/primitives/directional_light.obj", meshMaterial ),
    lightProperties_( lightProperties )
{}


DrawablePtr DirectionalLight::clone()
{
    return DrawablePtr( new DirectionalLight( *this ) );
}


/***
 * 3. Setters
 ***/

void DirectionalLight::update()
{
    // Update mesh associated to this light.
    Mesh::update();

    // Compute transformed light vector.
    lightProperties_->setLightVector( glm::normalize( glm::vec3( transformationMatrix * glm::vec4( 0.0f, -1.0f, 0.0f, 0.0f ) ) ) );
}


/***
 * 5. Lights management
 ***/

unsigned int DirectionalLight::getMaxLights()
{
    // TODO: Retrieve value from shader.
    return 4;
}

} // namespace como
