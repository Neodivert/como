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

#ifndef CYLINDER_CREATION_COMMAND_HPP
#define CYLINDER_CREATION_COMMAND_HPP

#include "geometric_primitive_command.hpp"
#include <common/packables/packable_float.hpp>

namespace como {

class CylinderCreationCommand : public GeometricPrimitiveCommand
{
    public:
        /***
         * 1. Construction
         ***/
        CylinderCreationCommand();
        CylinderCreationCommand( const ResourceID& cylinderID,
                             const ResourceID& materialID,
                             const ResourceID& firstTextureWallID,
                             float coneHeight,
                             float coneRadius,
                             unsigned int coneNBaseVertices,
                             const glm::vec3& centroid );
        CylinderCreationCommand( const CylinderCreationCommand& );
        CylinderCreationCommand( CylinderCreationCommand&& ) = delete;
        COMMAND_CLONE_METHOD( CylinderCreationCommand )


        /***
         * 2. Destruction
         ***/
        virtual ~CylinderCreationCommand() = default;


        /***
         * 3. Getters
         ***/
        float cylinderHeight() const;
        float cylinderRadius() const;
        std::uint16_t cylinderNRadialVertices() const;


        /***
         * 4. Operators
         ***/
        CylinderCreationCommand& operator = ( const CylinderCreationCommand& ) = delete;
        CylinderCreationCommand& operator = ( CylinderCreationCommand&& ) = delete;


    private:
        PackableFloat cylinderHeight_;
        PackableFloat cylinderRadius_;
        PackableUint16<std::uint16_t> cylinderNRadialVertices_;
};

} // namespace como

#endif // CYLINDER_CREATION_COMMAND_HPP
