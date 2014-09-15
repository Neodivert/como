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
#include <client/models/3d/meshes/imported_mesh.hpp>

namespace como {

/***
 * 1. Construction
 ***/

MeshesManager::MeshesManager( ServerInterfacePtr server, LogPtr log, MaterialsManagerPtr materialsManager ) :
    ResourceCommandsExecuter( server ),
    SpecializedEntitiesManager( server, log ),
    materialsManager_( materialsManager ),
    newMeshesDisplayVertexNormals_( false )
{
    displayEdges( MeshEdgesDisplayFrequency::ONLY_WHEN_SELECTED );
}


/***
 * 4. Getters
 ***/

string MeshesManager::getResourceName( const ResourceID& resourceID ) const
{
    for( const auto& meshesSelection : resourcesSelections_ ){
        if( meshesSelection.second->containsResource( resourceID ) ){
            return meshesSelection.second->getResourceName( resourceID );
        }
    }

    throw std::runtime_error( "MeshesManager::getResourceName() - resource not found" );
}


ElementsMeetingCondition MeshesManager::displaysVertexNormals() const
{
    const ElementsMeetingCondition firstSelectionValue =
            resourcesSelections_.begin()->second->displaysVertexNormals();

    if( firstSelectionValue == ElementsMeetingCondition::SOME ){
        return ElementsMeetingCondition::SOME;
    }

    for( const auto& meshesSelection : resourcesSelections_ ){
        if( meshesSelection.second->displaysVertexNormals() != firstSelectionValue ){
            return ElementsMeetingCondition::SOME;
        }
    }

    return firstSelectionValue;
}


unsigned int MeshesManager::getTotalMeshes() const
{
    unsigned int totalMeshes = 0;

    for( const auto& meshesSelection : resourcesSelections_ ){
        totalMeshes += meshesSelection.second->size();
    }

    return totalMeshes;
}


/***
 * 5. Setters
 ***/

void MeshesManager::displayVertexNormals( bool display )
{
    newMeshesDisplayVertexNormals_ = display;

    for( auto& meshesSelection : resourcesSelections_ ){
        meshesSelection.second->displayVertexNormals( display );
    }
}


void MeshesManager::displayEdges( MeshEdgesDisplayFrequency frequency )
{
    switch( frequency ){
        case MeshEdgesDisplayFrequency::ALWAYS:
            getResourcesSelection( NO_USER )->displayEdges( true );
        break;
        case MeshEdgesDisplayFrequency::ONLY_WHEN_SELECTED:
            getResourcesSelection( NO_USER )->displayEdges( false );
        break;
    }
}


/***
 * 6. Meshes management
 ***/

ResourceID MeshesManager::createMesh( const ImportedPrimitiveData& primitiveData, ConstMaterialsVector materials )
{    
    std::unique_ptr< Mesh > mesh( new ImportedMesh( primitiveData, materials, newMeshesDisplayVertexNormals_ ) );

    return addMesh( std::move( mesh ) );
}


void MeshesManager::createMesh( const ImportedPrimitiveData& primitiveData, ConstMaterialsVector materials, const ResourceID& meshID )
{
    std::unique_ptr< Mesh > mesh( new ImportedMesh( primitiveData, materials, newMeshesDisplayVertexNormals_ ) );

    addMesh( std::move( mesh ), meshID );
}


ResourceID MeshesManager::addMesh( MeshPtr mesh )
{
    ResourceID meshID = reserveResourceIDs( 1 );

    addMesh( std::move( mesh ), meshID );

    return meshID;
}


void MeshesManager::addMesh( MeshPtr mesh, const ResourceID& meshID )
{
    getResourcesSelection( meshID.getCreatorID() )->addResource( meshID, std::move( mesh ) );
}


/***
 * 4. Resources selections management
 ***/

void MeshesManager::registerUser( UserID userID )
{
    resourcesSelections_[ userID ];
}


void MeshesManager::removeUser( UserID userID )
{
    unlockResourcesSelection( userID );
    resourcesSelections_.erase( userID );
}


/***
 * 5. Locking
 ***/

void MeshesManager::lockResource( const ResourceID &resourceID, UserID newOwner )
{
    ResourcesManager::lockResource( resourceID, newOwner );

    materialsManager_->lockMeshMaterials( resourceID, newOwner );
}


void MeshesManager::unlockResourcesSelection( UserID currentOwner )
{
    ResourcesManager::unlockResourcesSelection( currentOwner );

    materialsManager_->unlockUserMaterials( currentOwner );
}


void MeshesManager::clearResourcesSelection( UserID currentOwner )
{
    ResourcesManager::clearResourcesSelection( currentOwner );

    materialsManager_->removeUserMaterials( currentOwner );
}


} // namespace como
