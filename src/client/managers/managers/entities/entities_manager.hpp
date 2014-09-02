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

#include <client/managers/managers/lights/lights_manager.hpp>
#include <client/managers/managers/meshes/meshes_manager.hpp>
#include <client/managers/selections/entities/local_entities_selection.hpp>
#include <client/managers/managers/entities/abstract_entities_manager.hpp>
#include <client/managers/managers/users/users_manager.hpp>

namespace como {

class EntitiesManager : public AbstractEntitiesManager, public ContainerObserver< UserID >
{
    public:
        /***
         * 1. Construction
         ***/
        EntitiesManager( ServerInterfacePtr server, LogPtr log, UsersManagerPtr usersManager, MaterialsManagerPtr materialsManager );
        EntitiesManager() = delete;
        EntitiesManager( const EntitiesManager& ) = delete;
        EntitiesManager( EntitiesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~EntitiesManager() = default;


        /***
         * 3. Selections management
         ***/
        void createUserSelection( UserID userID, const glm::vec4& selectionColor );
        void removeUserSelection();
        void removeUserSelection( UserID userID );


        /***
         * 4. Getters
         ***/
        LocalEntitiesSelection* getLocalSelection() const; // TODO: Return a undeletable pointer.
        MeshesManagerPtr getMeshesManager();
        LightsManagerPtr getLightsManager();
        virtual bool containsResource(const ResourceID &resourceID) const;


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
         * 8. Updating (observer pattern)
         ***/
        virtual void update( ContainerAction lastContainerAction, UserID modifiedUser );


        /***
         * 9. Operators
         ***/
        EntitiesManager& operator = ( const EntitiesManager& ) = default;
        EntitiesManager& operator = ( EntitiesManager&& ) = default;


    protected:
        /***
         * 10. Resources locking / unlocking
         ***/
        virtual void lockResource(const ResourceID &resourceID, UserID newOwner);
        virtual void unlockResourcesSelection(UserID currentOwner);
        virtual void clearResourcesSelection(UserID currentOwner);


    private:
        UsersManagerPtr usersManager_;

        std::map< UserID, std::unique_ptr<EntitiesSelection> > entitiesSelections_;

        MeshesManagerPtr meshesManager_;
        LightsManagerPtr lightsManager_;

        std::vector< AbstractEntitiesManager* > managers_;
};

typedef std::shared_ptr< EntitiesManager > EntitiesManagerPtr;

} // namespace como

#endif // ENTITIES_MANAGER_HPP
