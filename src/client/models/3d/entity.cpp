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

#include "entity.hpp"
#include <stdexcept>
#include <cstdlib>

namespace como {


/***
 * 1. Initialization and destruction
 ***/

Entity::Entity( DrawableType type, std::string name ) :
    type_( type ),
    name_( name ),
    modelMatrix_( 1.0f )
{
    // Initialize the drawable's original orientation.
    originalOrientation[X] = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
    originalOrientation[Y] = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
    originalOrientation[Z] = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );
}


/***
 * 2. Getters
 ***/

glm::mat4 Entity::getModelMatrix() const
{
    return modelMatrix_;
}


DrawableType Entity::getType() const
{
    return type_;
}


std::string Entity::getName() const
{
    return name_;
}


/***
 * 3. Transformations
 ***/

void Entity::applyTransformationMatrix( const glm::mat4& transformation )
{
    // Move the drawable from world to object space, then apply the new
    // transformation and finally move back the drawable from object to world
    // space.
    modelMatrix_ = transformation * modelMatrix_;

    // Update the transformed vertices using the original ones and the
    // previous transformation matrix.
    update();
}


/***
 * 5. Updating and drawing
 ***/

void Entity::update()
{
    for( unsigned int i = 0; i<3; i++ ){
        transformedOrientation[i] = modelMatrix_ * originalOrientation[i];
    }
}

} // namespace como.
