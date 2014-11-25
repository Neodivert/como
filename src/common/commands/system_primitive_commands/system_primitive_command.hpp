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

#ifndef SYSTEM_PRIMITIVE_COMMAND_HPP
#define SYSTEM_PRIMITIVE_COMMAND_HPP

#include <common/commands/type_command.hpp>
#include <common/packables/array/packable_array_3.hpp>
#include <common/packables/packable_float.hpp>

namespace como {

enum class SystemPrimitiveCommandType : std::uint8_t
{
    CUBE_CREATION = 0,
    CONE_CREATION,
    CYLINDER_CREATION,
    SPHERE_CREATION
};


class SystemPrimitiveCommand : public TypeCommand< SystemPrimitiveCommandType >
{
    public:
        /***
         * 1. Construction
         ***/
        SystemPrimitiveCommand( SystemPrimitiveCommandType type,
                                   const ResourceID& meshID,
                                   const ResourceID& materialID,
                                   const ResourceID& firstTextureWallID,
                                   std::uint8_t nTextureWalls,
                                   const glm::vec3& centroid = glm::vec3( 0.0f ) );
        SystemPrimitiveCommand() = delete;
        SystemPrimitiveCommand( const SystemPrimitiveCommand& );
        SystemPrimitiveCommand( SystemPrimitiveCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~SystemPrimitiveCommand() = default;


        /***
         * 3. Getters
         ***/
        ResourceID getMeshID() const;
        ResourceID getMaterialID() const;
        ResourceID getFirstTextureWallID() const;
        std::uint8_t nTextureWalls() const;
        glm::vec3 centroid() const;


        /***
         * 4. Operators
         ***/
        SystemPrimitiveCommand& operator = ( const SystemPrimitiveCommand& ) = delete;
        SystemPrimitiveCommand& operator = ( SystemPrimitiveCommand&& ) = delete;


    private:
        PackableResourceID meshID_;
        PackableResourceID materialID_;
        PackableResourceID firstTextureWallID_;
        PackableUint8< std::uint8_t > nTextureWalls_;
        PackableArray3< PackableFloat, float > centroid_;
};

} // namespace como

#endif // SYSTEM_PRIMITIVE_COMMAND_HPP
