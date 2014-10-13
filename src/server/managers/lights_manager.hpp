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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef LIGHTS_MANAGER_HPP
#define LIGHTS_MANAGER_HPP

#include <set>
#include <common/ids/resource_id.hpp>
#include <common/commands/light_commands/light_commands.hpp>
#include <common/utilities/observable_container/container_observer.hpp>
#include <server/commands_historic.hpp>
#include <server/resources_ownership_manager.hpp>
#include <common/utilities/lockable.hpp>

namespace como {

class LightsManager : public ContainerObserver<ResourceID>, public Lockable
{
    public:
        /***
         * 1. Construction
         ***/
        LightsManager( unsigned int maxDirectionalLights,
                       ResourcesOwnershipManager* resourcesOwnershipManager );

        LightsManager() = delete;
        LightsManager( const LightsManager& ) = delete;
        LightsManager( LightsManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LightsManager() = default;


        /***
         * 3. Updating (Observer pattern)
         ***/
        virtual void update( ContainerAction lastContainerAction, ResourceID lastElementModified );


        /***
         * 4. Lights management
         ***/
        bool requestDirectionalLightCreation( const ResourceID& lightID );


        /***
         * 5. Operators
         ***/
        LightsManager& operator = ( const LightsManager& ) = delete;
        LightsManager& operator = ( LightsManager&& ) = delete;


    private:
        const unsigned int MAX_LIGHTS;
        const unsigned int MAX_DIRECTIONAL_LIGHTS;

        ResourcesOwnershipManager* resourcesOwnershipManager_;
        CommandsHistoric* commandsHistoric_;

        std::set< ResourceID > lights_;
};

} // namespace como

#endif // LIGHTS_MANAGER_HPP
