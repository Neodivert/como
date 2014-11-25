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

#ifndef CONE_CREATION_COMMAND_HPP
#define CONE_CREATION_COMMAND_HPP

#include "system_primitive_command.hpp"
#include <common/packables/packable_float.hpp>

namespace como {

class ConeCreationCommand : public SystemPrimitiveCommand
{
    public:
        /***
         * 1. Construction
         ***/
        ConeCreationCommand();
        ConeCreationCommand( const ResourceID& coneID,
                             const ResourceID& materialID,
                             const ResourceID& firstTextureWallID,
                             float coneHeight,
                             float coneRadius,
                             unsigned int coneNBaseVertices,
                             const glm::vec3& centroid );
        ConeCreationCommand( const ConeCreationCommand& );
        ConeCreationCommand( ConeCreationCommand&& ) = delete;
        COMMAND_CLONE_METHOD( ConeCreationCommand )


        /***
         * 2. Destruction
         ***/
        virtual ~ConeCreationCommand() = default;


        /***
         * 3. Getters
         ***/
        float coneHeight() const;
        float coneRadius() const;
        std::uint16_t coneNBaseVertices() const;


        /***
         * 4. Operators
         ***/
        ConeCreationCommand& operator = ( const ConeCreationCommand& ) = delete;
        ConeCreationCommand& operator = ( ConeCreationCommand&& ) = delete;


    private:
        PackableFloat coneHeight_;
        PackableFloat coneRadius_;
        PackableUint16<std::uint16_t> coneNBaseVertices_;
};

} // namespace como

#endif // CONE_CREATION_COMMAND_HPP
