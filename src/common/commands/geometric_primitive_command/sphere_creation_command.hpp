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

#ifndef SPHERE_CREATION_COMMAND_HPP
#define SPHERE_CREATION_COMMAND_HPP

#include "geometric_primitive_command.hpp"
#include <common/packables/packable_float.hpp>
#include <common/packables/packable_integer.hpp>

namespace como {

class SphereCreationCommand : public GeometricPrimitiveCommand
{
    public:
        /***
         * 1. Construction
         ***/
        SphereCreationCommand();
        SphereCreationCommand( const ResourceID& sphereID,
                               const ResourceID& materialID,
                               const ResourceID& firstTextureWallID,
                               float sphereRadius,
                               std::uint16_t sphereNDivisions,
                               const glm::vec3& centroid );
        SphereCreationCommand( const SphereCreationCommand& );
        SphereCreationCommand( SphereCreationCommand&& ) = delete;
        COMMAND_CLONE_METHOD( SphereCreationCommand )


        /***
         * 2. Destruction
         ***/
        virtual ~SphereCreationCommand() = default;


        /***
         * 3. Getters
         ***/
        float sphereRadius() const;
        std::uint16_t sphereNDivisions() const;


        /***
         * 4. Operators
         ***/
        SphereCreationCommand& operator = ( const SphereCreationCommand& ) = delete;
        SphereCreationCommand& operator = ( SphereCreationCommand&& ) = delete;


    private:
        PackableFloat sphereRadius_;
        PackableUint16<std::uint16_t> sphereNDivisions_;
};

} // namespace como

#endif // SPHERE_CREATION_COMMAND_HPP
