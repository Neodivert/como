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

#ifndef ABSTRACT_ENTITIES_MANAGER_HPP
#define ABSTRACT_ENTITIES_MANAGER_HPP

#include <client/managers/resources_manager.hpp>

namespace como {

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
class AbstractEntitiesManager : public ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType >
{
    public:
        /***
         * 1. Construction
         ***/
        AbstractEntitiesManager( ServerInterfacePtr server, LogPtr log );
        AbstractEntitiesManager() = default;
        AbstractEntitiesManager( AbstractEntitiesManager& ) = default;
        AbstractEntitiesManager( AbstractEntitiesManager&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~AbstractEntitiesManager() = default;


        /***
         * 3. Entities picking
         ***/
        virtual ResourceID selectEntityByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection, glm::vec3& worldCollisionPoint );


        /***
         * 4. Operators
         ***/
        AbstractEntitiesManager& operator = ( const AbstractEntitiesManager& ) = default;
        AbstractEntitiesManager& operator = ( AbstractEntitiesManager&& ) = default;
};


/***
 * 1. Construction
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
AbstractEntitiesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::AbstractEntitiesManager( ServerInterfacePtr server, LogPtr log ) :
    ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>( server, log )
{}


/***
 * 5. Entities picking
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
ResourceID AbstractEntitiesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::selectEntityByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection, glm::vec3& worldCollisionPoint )
{
    const float MAX_T = 9999999.0f;
    float minT = MAX_T;
    ResourceID closestObject;

    r1 = glm::normalize( r1 );

    // Does the user want to keep the actual set of selected objects and simply add
    // a new one? If that's NOT the case, we need to clear the set of selected drawables
    // first.
    if( !addToSelection ){
        this->getLocalResourcesSelection()->moveAll( *( this->getResourcesSelection( NO_USER ) ) );
        // TODO: Make this: this->requestSelectionUnlock( this->localUserID() );
    }

    // Check if the given ray intersect any of the non selected drawables.
    if( this->getResourcesSelection( NO_USER )->intersectsRay( r0, r1, closestObject, minT ) ){
        // A non selected drawable has been intersected.
        this->log()->debug( "Object picked\n" );

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
            this->log()->debug( "RETURN 0\n" );
            ////emit renderNeeded();
            return NO_RESOURCE;
        }else{
            this->log()->debug( "NO CLOSEST OBJECT. Unselecting all\n" );
            this->getLocalResourcesSelection()->moveAll( *( this->getResourcesSelection( NO_USER ) ) );
            // TODO: Make this: this->requestSelectionUnlock( this->localUserID() );
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


} // namespace como

#endif // ABSTRACT_ENTITIES_MANAGER_HPP
