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

#ifndef ENTITIES_MANAGER_HPP
#define ENTITIES_MANAGER_HPP

#include <client/managers/lights/lights_manager.hpp>
#include <client/managers/meshes_manager.hpp>

namespace como {

class EntitiesManager
{
    public:
        /***
         * 1. Construction
         ***/
        EntitiesManager( ServerInterfacePtr server, LogPtr log );
        EntitiesManager() = delete;
        EntitiesManager( const EntitiesManager& ) = delete;
        EntitiesManager( EntitiesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~EntitiesManager() = default;


        /***
         * 3. Getters
         ***/
        MeshesManager* getMeshesManager();
        LightsManager* getLightsManager();


        /***
         * 4. Operators
         ***/
        EntitiesManager& operator = ( const EntitiesManager& ) = default;
        EntitiesManager& operator = ( EntitiesManager&& ) = default;


    private:
        MeshesManager meshesManager_;
        LightsManager lightsManager_;
};

} // namespace como

#endif // ENTITIES_MANAGER_HPP
