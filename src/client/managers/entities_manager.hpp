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
        MeshesManagerPtr getMeshesManager();
        LightsManagerPtr getLightsManager();


        /***
         * 4. Entity picking
         ***/
        ResourceID selectEntityByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection, glm::vec3& worldCollisionPoint );


        /***
         * 5. Drawing
         ***/
        void drawAll( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const;


        /***
         * 6. Operators
         ***/
        EntitiesManager& operator = ( const EntitiesManager& ) = default;
        EntitiesManager& operator = ( EntitiesManager&& ) = default;


    private:
        MeshesManagerPtr meshesManager_;
        LightsManagerPtr lightsManager_;
};

typedef std::shared_ptr< EntitiesManager > EntitiesManagerPtr;

} // namespace como

#endif // ENTITIES_MANAGER_HPP
