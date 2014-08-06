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

#include "meshes_manager.hpp"

namespace como {

/***
 * 1. Construction
 ***/

MeshesManager::MeshesManager( ServerInterfacePtr server, DrawablesManagerPtr drawablesManager ) :
    ResourcesManager( server ),
    drawablesManager_( drawablesManager )
{
    // Create a selection of non selected meshes and keep a pointer to it.
    meshesSelections_[NO_USER];
    nonSelectedMeshes_ = &( meshesSelections_.at( NO_USER ) );
}


/***
 * 4. Resources ownership management
 ***/

void MeshesManager::lockResource( const ResourceID& resourceID, UserID userID )
{
    if( !nonSelectedMeshes_->moveMesh( resourceID, meshesSelections_.at( userID ) ) ){
        throw std::runtime_error( "MeshesManager::lockResource() - Requested resource NOT FOUND" );
    }
}


void MeshesManager::unlockResourcesSelection( UserID userID )
{
    meshesSelections_.at( userID ).moveAll( *nonSelectedMeshes_ );
}


void MeshesManager::deleteResourcesSelection( UserID userID )
{
    meshesSelections_.erase( userID );
}

} // namespace como
