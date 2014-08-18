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

#include "entities_manager.hpp"

namespace como {


/***
 * 1. Construction
 ***/

EntitiesManager::EntitiesManager( ServerInterfacePtr server, LogPtr log ) :
    meshesManager_( new MeshesManager( server, log ) ),
    lightsManager_( new LightsManager( server, log ) )
{
    entitiesSelections_[NO_USER] = EntitiesSelectionPtr( new EntitiesSelection( lightsManager_->getResourcesSelection( NO_USER ).get() ) );
    entitiesSelections_[server->getLocalUserID()] = EntitiesSelectionPtr( new EntitiesSelection( lightsManager_->getLocalResourcesSelection().get() ) );
}


/***
 * 3. Selections management
 ***/

void EntitiesManager::createUserSelection( UserID userID )
{
    lightsManager_->createResourcesSelection( userID );
    meshesManager_->createResourcesSelection( userID );

    entitiesSelections_[userID] = EntitiesSelectionPtr( new EntitiesSelection( lightsManager_->getResourcesSelection( userID ).get() ) );
}


void EntitiesManager::removeUserSelection( UserID userID )
{
    entitiesSelections_.erase( userID );
}


/***
 * 4. Getters
 ***/

MeshesManagerPtr EntitiesManager::getMeshesManager()
{
    return meshesManager_;
}


LightsManagerPtr EntitiesManager::getLightsManager()
{
    return lightsManager_;
}


/***
 * 5. Entity picking
 ***/

ResourceID EntitiesManager::selectEntityByRayPicking(glm::vec3 r0, glm::vec3 r1, bool addToSelection, glm::vec3& worldCollisionPoint )
{
    // TODO: Call also to MeshesManager::selectEntityByRayPicking() and lock
    // only the closest entity between both managers.
    return lightsManager_->selectEntityByRayPicking( r0, r1, addToSelection, worldCollisionPoint );
}


/***
 * 6. Drawing
 ***/

void EntitiesManager::drawAll( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const
{
    lightsManager_->drawAll( openGL, viewMatrix, projectionMatrix );
}

} // namespace como
