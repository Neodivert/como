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

#include "model_matrix_replacement_command.hpp"
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

namespace como {

/***
 * 1. Construction
 ***/

ModelMatrixReplacementCommand::ModelMatrixReplacementCommand() :
    EntityCommand( EntityCommandType::MODEL_MATRIX_REPLACEMENT, NO_RESOURCE, NO_USER ),
    modelMatrix_( 0.0f )
{
    addPackable( &modelMatrix_ );
}


ModelMatrixReplacementCommand::ModelMatrixReplacementCommand(const ResourceID &entityID, UserID userID, const glm::mat4 &modelMatrix) :
    EntityCommand( EntityCommandType::MODEL_MATRIX_REPLACEMENT, entityID, userID ),
    modelMatrix_( glm::value_ptr( modelMatrix ) )
{
    addPackable( &modelMatrix_ );
}

ModelMatrixReplacementCommand::ModelMatrixReplacementCommand( const ModelMatrixReplacementCommand &b ) :
    EntityCommand( b ),
    modelMatrix_( b.modelMatrix_ )
{
    addPackable( &modelMatrix_ );
}


/***
 * 3. Getters
 ***/

glm::mat4 ModelMatrixReplacementCommand::modelMatrix() const
{
    std::array< float, 16 > v = modelMatrix_.getValues();

    return glm::mat4(
            v[0], v[1], v[2], v[3],
            v[4], v[5], v[6], v[7],
            v[8], v[9], v[10], v[11],
            v[12], v[13], v[14], v[15]
            );
}

} // namespace como
