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

#ifndef SPECIALIZED_ENTITIES_MANAGER_HPP
#define SPECIALIZED_ENTITIES_MANAGER_HPP

#include <client/managers/resources_manager.hpp>

namespace como {

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
class SpecializedEntitiesManager : public ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType >
{
    public:
        /***
         * 1. Construction
         ***/
        SpecializedEntitiesManager( ServerInterfacePtr server, LogPtr log );
        SpecializedEntitiesManager() = default;
        SpecializedEntitiesManager( SpecializedEntitiesManager& ) = default;
        SpecializedEntitiesManager( SpecializedEntitiesManager&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~SpecializedEntitiesManager() = default;


        /***
         * 3. Entities picking
         ***/
        virtual ResourceID selectEntityByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection, glm::vec3& worldCollisionPoint );


        /***
         * 4. Drawing
         ***/
        void drawAll( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const;


        /***
         * 5. Operators
         ***/
        SpecializedEntitiesManager& operator = ( const SpecializedEntitiesManager& ) = default;
        SpecializedEntitiesManager& operator = ( SpecializedEntitiesManager&& ) = default;
};


/***
 * 1. Construction
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
SpecializedEntitiesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::SpecializedEntitiesManager( ServerInterfacePtr server, LogPtr log ) :
    ResourceCommandsExecuter( server ),
    ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>( server, log )
{}


/***
 * 3. Entities picking
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
ResourceID SpecializedEntitiesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::selectEntityByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection, glm::vec3& worldCollisionPoint )
{
    const float MAX_T = 9999999.0f;
    float minT = MAX_T;
    ResourceID closestObject;

    r1 = glm::normalize( r1 );

    // Does the user want to keep the actual set of selected objects and simply add
    // a new one? If that's NOT the case, we need to clear the set of selected drawables
    // first.
    if( !addToSelection ){
        this->requestSelectionUnlock();
    }

    // Check if the given ray intersect any of the non selected drawables.
    if( this->getResourcesSelection( NO_USER )->intersectsRay( r0, r1, closestObject, minT ) ){
        // A non selected drawable has been intersected.
        //this->log()->debug( "Object picked\n" );

        // Request to the server the lock of the intersected drawable.
        // TODO: Request resource lock in the caller. I'll call
        // selectEntityByRayPicking() on multiple managers.
        this->requestResourceLock( closestObject );

        // Save the collision point (in world coordinates) for returning it to
        // caller.
        worldCollisionPoint = r0 + r1 * minT;
    }else{
        // If user dind't selected any non-selected drawable, check if he / she
        // clicked on an already selected one.
        if( this->getLocalResourcesSelection()->intersectsRay( r0, r1, closestObject, minT ) ){
            //this->log()->debug( "RETURN 0\n" );
            ////emit renderNeeded();
            return NO_RESOURCE;
        }else{
            //this->log()->debug( "NO CLOSEST OBJECT. Unselecting all\n" );
            this->requestSelectionUnlock();
        }

        // Even if no object is collided, we return in "worldCollisionPoint"
        // the "collision" with the near plane.
        //worldCollisionPoint = r0;
        worldCollisionPoint = glm::vec3( 0.0f );
    }

    // TODO: Send a request to server even when trying to select another
    // user's drawables?

    return closestObject;
}


/***
 * 4. Drawing
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void SpecializedEntitiesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::drawAll( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const
{
    for( auto entitiesSelectionPair : this->resourcesSelections_ ){
        entitiesSelectionPair.second->drawAll( openGL, viewMatrix, projectionMatrix );
    }
}

} // namespace como

#endif // SPECIALIZED_ENTITIES_MANAGER_HPP
