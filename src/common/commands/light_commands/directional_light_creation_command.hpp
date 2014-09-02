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

#ifndef DIRECTIONAL_LIGHT_CREATIONCOMMAND_HPP
#define DIRECTIONAL_LIGHT_CREATIONCOMMAND_HPP

#include "light_creation_command.hpp"

namespace como {

class DirectionalLightCreationCommand : public LightCreationCommand
{
    public:
        /***
         * 1. Construction
         ***/
        DirectionalLightCreationCommand();
        DirectionalLightCreationCommand( UserID userID, ResourceID lightID, const PackableColor& lightColor );
        DirectionalLightCreationCommand( const DirectionalLightCreationCommand& ) = default;
        DirectionalLightCreationCommand( DirectionalLightCreationCommand&& ) = delete;
        COMMAND_CLONE_METHOD( DirectionalLightCreationCommand )


        /***
         * 2. Destruction
         ***/
        virtual ~DirectionalLightCreationCommand() = default;


        /***
         * 3. Operators
         ***/
        DirectionalLightCreationCommand& operator=( const DirectionalLightCreationCommand& ) = delete;
        DirectionalLightCreationCommand& operator=( DirectionalLightCreationCommand&& ) = delete;
};

} // namespace como

#endif // DIRECTIONAL_LIGHT_CREATIONCOMMAND_HPP
