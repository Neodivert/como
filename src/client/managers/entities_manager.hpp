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
#include <client/managers/selections/entities/local_entities_selection.hpp>
#include <client/managers/abstract_entities_manager.hpp>

namespace como {

class EntitiesManager : public AbstractEntitiesManager
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
         * 3. Selections management
         ***/
        void createUserSelection( const UserConnectionCommand* userConnectionCommand );
        void removeUserSelection();
        void removeUserSelection( UserID userID );


        /***
         * 4. Getters
         ***/
        LocalEntitiesSelectionPtr getLocalSelection() const;
        MeshesManagerPtr getMeshesManager();
        LightsManagerPtr getLightsManager();


        /***
         * 5. Entity picking
         ***/
        virtual bool pick( const glm::vec3 &rayOrigin, glm::vec3 rayDirection, ResourceID &pickedElement, float &t, const float &MAX_T = FLT_MAX ) const;


        /***
         * 6. Command execution
         ***/
        void executeRemoteSelectionCommand( SelectionCommandConstPtr command );
        void executeRemoteParameterChangeCommand( UserParameterChangeCommandConstPtr command );


        /***
         * 7. Drawing
         ***/
        void drawAll( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const;


        /***
         * 8. Operators
         ***/
        EntitiesManager& operator = ( const EntitiesManager& ) = default;
        EntitiesManager& operator = ( EntitiesManager&& ) = default;


    protected:
        /***
         * 9. Resources locking / unlocking
         ***/
        virtual void lockResource(const ResourceID &resourceID, UserID newOwner);
        virtual void unlockResourcesSelection(UserID currentOwner);
        virtual void clearResourcesSelection(UserID currentOwner);


    private:
        ServerInterfacePtr server_;

        std::map< UserID, EntitiesSelectionPtr > entitiesSelections_;

        MeshesManagerPtr meshesManager_;
        LightsManagerPtr lightsManager_;

        std::vector< std::shared_ptr< ResourcesOwnershipManager > > managers_;
};

typedef std::shared_ptr< EntitiesManager > EntitiesManagerPtr;

} // namespace como

#endif // ENTITIES_MANAGER_HPP
