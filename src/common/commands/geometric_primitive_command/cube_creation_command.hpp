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

#ifndef CUBE_CREATION_COMMAND_HPP
#define CUBE_CREATION_COMMAND_HPP

#include "geometric_primitive_command.hpp"
#include <common/packables/packable_float.hpp>

namespace como {

class CubeCreationCommand : public GeometricPrimitiveCommand
{
    public:
        /***
         * 1. Construction
         ***/
        CubeCreationCommand();
        CubeCreationCommand( const ResourceID& cubeID,
                             const ResourceID& materialID,
                             const ResourceID& firstTextureWallID,
                             float width,
                             float height,
                             float depth,
                             const glm::vec3& centroid );
        CubeCreationCommand( const CubeCreationCommand& );
        CubeCreationCommand( CubeCreationCommand&& ) = delete;
        COMMAND_CLONE_METHOD( CubeCreationCommand )


        /***
         * 2. Destruction
         ***/
        ~CubeCreationCommand() = default;


        /***
         * 3. Getters
         ***/
        float getCubeWidth() const;
        float getCubeHeight() const;
        float getCubeDepth() const;


        /***
         * 4. Operators
         ***/
        CubeCreationCommand& operator = ( const CubeCreationCommand& ) = delete;
        CubeCreationCommand& operator = ( CubeCreationCommand&& ) = delete;


    private:
        PackableFloat cubeWidth_;
        PackableFloat cubeHeight_;
        PackableFloat cubeDepth_;
};

} // namespace como

#endif // CUBE_CREATION_COMMAND_HPP
