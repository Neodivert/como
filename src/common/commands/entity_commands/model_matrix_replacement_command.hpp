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

#ifndef MODEL_MATRIX_REPLACEMENT_COMMAND_HPP
#define MODEL_MATRIX_REPLACEMENT_COMMAND_HPP

#include "entity_command.hpp"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <common/packables/array/packable_array.hpp>
#include <common/packables/packable_float.hpp>

namespace como {

class ModelMatrixReplacementCommand : public EntityCommand
{
    public:
        /***
         * 1. Construction
         ***/
        ModelMatrixReplacementCommand();
        ModelMatrixReplacementCommand( const ResourceID& entityID, UserID userID, const glm::mat4& modelMatrix );
        ModelMatrixReplacementCommand( const ModelMatrixReplacementCommand& );
        ModelMatrixReplacementCommand( ModelMatrixReplacementCommand&& ) = delete;
        COMMAND_CLONE_METHOD( ModelMatrixReplacementCommand )


        /***
         * 2. Destruction
         ***/
        virtual ~ModelMatrixReplacementCommand() = default;


        /***
         * 3. Getters
         ***/
        glm::mat4 modelMatrix() const;


        /***
         * 4. Operators
         ***/
        ModelMatrixReplacementCommand& operator = ( const ModelMatrixReplacementCommand& ) = delete;
        ModelMatrixReplacementCommand& operator = ( ModelMatrixReplacementCommand&& ) = delete;


    private:
        PackableArray< PackableFloat, float, 16 > modelMatrix_;
};

} // namespace como

#endif // MODEL_MATRIX_REPLACEMENT_COMMAND_HPP
