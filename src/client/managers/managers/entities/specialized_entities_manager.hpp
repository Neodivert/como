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

#include <client/managers/managers/resources/resources_manager.hpp>
#include <client/managers/managers/entities/abstract_entities_manager.hpp>

namespace como {

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
class SpecializedEntitiesManager : public AbstractEntitiesManager, public ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType >
{
    friend class EntitiesManager; // TODO: A better approach?

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
        virtual ~SpecializedEntitiesManager() = default;


        /***
         * 3. Getters
         ***/
        virtual std::string getResourceName(const ResourceID &resourceID) const;
        virtual bool containsResource( const ResourceID& resourceID ) const;


        /***
         * 4. Entities picking
         ***/
        virtual bool pick( const glm::vec3 &rayOrigin, glm::vec3 rayDirection, ResourceID &closestObject, float &t, const float &MAX_T = FLT_MAX ) const;


        /***
         * 5. Drawing
         ***/
        void drawAll( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const;


        /***
         * 6. Operators
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
    AbstractEntitiesManager( server ),
    ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>( server, log )
{}


/***
 * 3. Getters
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
std::string SpecializedEntitiesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::getResourceName(const ResourceID &resourceID) const
{
    LOCK
    (void)( resourceID );
    return "light";
}


template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
bool SpecializedEntitiesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::containsResource(const ResourceID &resourceID) const
{
    LOCK
    return this->ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::containsResource( resourceID );
}


/***
 * 4. Entities picking
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
bool SpecializedEntitiesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::pick(const glm::vec3 &rayOrigin, glm::vec3 rayDirection, ResourceID& closestObject, float &t, const float &MAX_T) const
{
    LOCK

    t = MAX_T;

    // Check if the given ray intersect any of the non selected drawables.
    return this->getResourcesSelection( NO_USER )->intersectsRay( rayOrigin, rayDirection, closestObject, t );

    // TODO: Send a request to server even when trying to select another
    // user's drawables?
}


/***
 * 4. Drawing
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void SpecializedEntitiesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::drawAll( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const
{
    LOCK
    for( const auto& entitiesSelectionPair : this->resourcesSelections_ ){
        entitiesSelectionPair.second->drawAll( openGL, viewMatrix, projectionMatrix );
    }
}

} // namespace como

#endif // SPECIALIZED_ENTITIES_MANAGER_HPP
