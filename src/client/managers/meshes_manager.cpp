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
    drawablesManager_( drawablesManager ),
    newMeshesDisplayVertexNormals_( false )
{
    // Create a selection of non selected meshes and keep a pointer to it.
    meshesSelections_[NO_USER];
    nonSelectedMeshes_ = &( meshesSelections_.at( NO_USER ) );
}


/***
 * 4. Getters
 ***/

ElementsMeetingCondition MeshesManager::displaysVertexNormals() const
{
    const ElementsMeetingCondition firstSelectionValue =
            meshesSelections_.begin()->second.displaysVertexNormals();

    if( firstSelectionValue == ElementsMeetingCondition::SOME ){
        return ElementsMeetingCondition::SOME;
    }

    for( auto meshesSelection : meshesSelections_ ){
        if( meshesSelection.second.displaysVertexNormals() != firstSelectionValue ){
            return ElementsMeetingCondition::SOME;
        }
    }

    return firstSelectionValue;
}


unsigned int MeshesManager::getTotalMeshes() const
{
    unsigned int totalMeshes = 0;

    for( auto meshesSelection : meshesSelections_ ){
        totalMeshes += meshesSelection.second.size();
    }

    return totalMeshes;
}


MeshesSelection* MeshesManager::getLocalUserSelection()
{
    return &( meshesSelections_.at( localUserID() ) );
}


/***
 * 5. Setters
 ***/

void MeshesManager::displayVertexNormals( bool display )
{
    newMeshesDisplayVertexNormals_ = display;

    for( auto meshesSelection : meshesSelections_ ){
        meshesSelection.second.displayVertexNormals( display );
    }
}


/***
 * 6. Meshes management
 ***/

ResourceID MeshesManager::createMesh( MeshVertexData vertexData, MeshOpenGLData oglData, const std::vector< PolygonGroupData >& polygonsGroups, const std::vector< MaterialConstPtr >& materials )
{
    MeshPtr mesh( new Mesh( vertexData, oglData, polygonsGroups, materials, newMeshesDisplayVertexNormals_ ) );

    ResourceID meshID = drawablesManager_->addDrawable( mesh );
    getLocalUserSelection()->addMesh( meshID, mesh ); // TODO: Or I have add it to the non selected meshes selection?

    return meshID;
}


void MeshesManager::createMesh( ResourceID meshID, MeshVertexData vertexData, MeshOpenGLData oglData, const std::vector< PolygonGroupData >& polygonsGroups, const std::vector< MaterialConstPtr >& materials )
{
    MeshPtr mesh( new Mesh( vertexData, oglData, polygonsGroups, materials, newMeshesDisplayVertexNormals_ ) );

    meshesSelections_.at( meshID.getCreatorID() ).addMesh( meshID, mesh );
    drawablesManager_->addDrawable( meshID.getCreatorID(), mesh, meshID );
}



/***
 * 4. Resources selections management
 ***/

void MeshesManager::registerUser( UserID userID )
{
    meshesSelections_[ userID ];
}

void MeshesManager::removeUser( UserID userID )
{
    unlockResourcesSelection( userID );
    meshesSelections_.erase( userID );
}


/***
 * 5. Resources ownership management
 ***/

void MeshesManager::lockResource( const ResourceID& resourceID, UserID userID )
{
    nonSelectedMeshes_->moveMesh( resourceID, meshesSelections_.at( userID ) );
}


void MeshesManager::unlockResourcesSelection( UserID userID )
{
    meshesSelections_.at( userID ).moveAll( *nonSelectedMeshes_ );
}


void MeshesManager::deleteResourcesSelection( UserID userID )
{
    meshesSelections_.at( userID ).clear();
}

} // namespace como
