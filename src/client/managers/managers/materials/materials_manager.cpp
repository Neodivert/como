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

#include "materials_manager.hpp"

namespace como {

/***
 * 1. Creation
 ***/

MaterialsManager::MaterialsManager( ServerInterfacePtr server, LogPtr log ) :
    ServerWriter( server )
{
    (void)( log ); // TODO: Remove this argument.
}


/***
 * 3. Material creation
 ***/

ResourceID MaterialsManager::createMaterials( const std::vector< MaterialInfo >& materialsInfo, const ResourceID &meshID )
{
    LOCK

    ResourceID firstMaterialID = reserveResourceIDs( 1 );

    createMaterials( materialsInfo, firstMaterialID, meshID );

    return firstMaterialID;
}


ResourceID MaterialsManager::createMaterial( const MaterialInfo& materialInfo, const ResourceID &meshID )
{
    LOCK

    ResourceID materialID = reserveResourceIDs( 1 );

    // Add the new material to the materials container.
    createMaterial( materialInfo, materialID, meshID );

    return materialID;
}


void MaterialsManager::createMaterials( const std::vector< MaterialInfo >& materialsInfo, const ResourceID &firstMaterialID, const ResourceID& meshID)
{
    LOCK
    ResourceID materialID = firstMaterialID;

    for( const auto& materialInfo : materialsInfo ){
        createMaterial( materialInfo, materialID, meshID );
        materialID++;
    }
}


void MaterialsManager::createMaterial( const MaterialInfo& materialInfo, const ResourceID &materialID, const ResourceID& meshID )
{
    LOCK
    materials_[materialID] = MaterialPtr( new Material( materialID, materialInfo ) );

    if( meshMaterials_.count( meshID ) == 0 ){
        meshMaterials_[meshID] = std::vector<ResourceID>();
    }
    meshMaterials_.at( meshID ).push_back( materialID );

    // Set the creator of the material as its current owner.
    lockMaterial( materialID, materialID.getCreatorID() );

    notifyElementInsertion( materialID );
}


/***
 * 4. Material selection
 ***/

MaterialHandlerPtr MaterialsManager::selectMaterial( const ResourceID& id )
{
    LOCK
    if( materialsOwners_.at( id ) != localUserID() ){
        throw std::runtime_error( "You don't have permission for selecting this material!" );
    }

    // TODO: Ask for permission to server once we can share materials among
    // meshes.

    if( materialHandler_ ){
        materialHandler_->removeObserver( this );
    }

    materialHandler_ = MaterialHandlerPtr( new MaterialHandler( id, materials_.at( id ), server() ) );

    materialHandler_->addObserver( this );

    return materialHandler_;
}


/***
 * 5. Getters
 ***/

ResourceHeadersList MaterialsManager::getLocalMaterialsHeaders() const
{
    LOCK
    ResourceHeadersList headers;

    for( const auto& materialPair : materials_ ){
        if( materialsOwners_.at( materialPair.first ) == localUserID() ){
            headers.push_back( materialPair.second->header() );
        }
    }

    return headers;
}


bool MaterialsManager::materialOwnedByLocalUser( const ResourceID& resourceID ) const
{
    LOCK
    return ( materialsOwners_.at( resourceID ) == localUserID() );
}

// TODO: Remove this method and use ResourcesManager::resourceName() instead.
std::string MaterialsManager::getResourceName( const ResourceID& resourceID ) const
{
    LOCK
    (void)( resourceID );
    return "Material";
}


MaterialConstPtr MaterialsManager::getMaterial( const ResourceID& id ) const
{
    LOCK
    return materials_.at( id );
}

ConstMaterialsVector MaterialsManager::getMaterials( const ResourceID& firstMaterialID, unsigned int nMaterials ) const
{
    LOCK
    ConstMaterialsVector materials;
    unsigned int i = 0;
    ResourceID materialID = firstMaterialID;

    materials.reserve( nMaterials );

    for( i = 0; i < nMaterials; i++ ){
        materials.push_back( materials_.at( materialID ) );
        materialID++;
    }

    return materials;
}


bool MaterialsManager::materialIncludesTexture( const ResourceID& materialID ) const
{
    LOCK
    return materials_.at( materialID )->includesTexture();
}


/***
 * 6. Remote command execution
 ***/

void MaterialsManager::executeRemoteCommand( const MaterialCommand& command )
{
    LOCK

    switch( command.getType() ){
        case MaterialCommandType::MATERIAL_CREATION:{
            throw std::runtime_error( "MATERIAL_CREATION commands aren't supported in this version of COMO" );
        }break;

        case MaterialCommandType::MATERIAL_MODIFICATION:{
            const AbstractMaterialModificationCommand& materialModificationCommand =
                    dynamic_cast< const AbstractMaterialModificationCommand& >( command );

            switch( materialModificationCommand.getParameterName() ){
                case MaterialParameterName::COLOR:{
                    const MaterialColorChangeCommand& materialColorChange =
                            dynamic_cast< const MaterialColorChangeCommand& >( materialModificationCommand );

                    materials_.at( command.getMaterialID() )->setColor( materialColorChange.getParameterValue() );
                }break;

                case MaterialParameterName::AMBIENT_REFLECTIVITY:{
                    const MaterialAmbientReflectivityChangeCommand& ambientReflexitivyChange =
                            dynamic_cast< const MaterialAmbientReflectivityChangeCommand& >( materialModificationCommand );

                    materials_.at( command.getMaterialID() )->setAmbientReflectivity( ambientReflexitivyChange.getParameterValue() );
                }break;

                case MaterialParameterName::DIFFUSE_REFLECTIVITY:{
                    const MaterialDiffuseReflectivityChangeCommand& diffuseReflexitivyChange =
                            dynamic_cast< const MaterialDiffuseReflectivityChangeCommand& >( materialModificationCommand );

                    materials_.at( command.getMaterialID() )->setDiffuseReflectivity( diffuseReflexitivyChange.getParameterValue() );
                }break;

                case MaterialParameterName::SPECULAR_REFLECTIVITY:{
                    const MaterialSpecularReflectivityChangeCommand& specularReflexitivyChange =
                            dynamic_cast< const MaterialSpecularReflectivityChangeCommand& >( materialModificationCommand );

                    materials_.at( command.getMaterialID() )->setSpecularReflectivity( specularReflexitivyChange.getParameterValue() );
                }break;

                case MaterialParameterName::SPECULAR_EXPONENT:{
                    const MaterialSpecularExponentChangeCommand& specularExponentChange =
                            dynamic_cast< const MaterialSpecularExponentChangeCommand& >( materialModificationCommand );

                    materials_.at( command.getMaterialID() )->setSpecularExponent( specularExponentChange.getParameterValue() );
                }break;
            }

            //notifyElementModification( command->getMaterialID() );

            notifyObservers();
        }break;
    }
}


/***
 * 7. Updating (pattern Observer)
 ***/

void MaterialsManager::update()
{
    LOCK
    notifyObservers();
}


/***
 * 8. Shader communication
 ***/

void MaterialsManager::sendMaterialToShader( const ResourceID &materialID )
{
    LOCK
    materials_.at( materialID )->sendToShader();
}


/***
 * 10. Materials locking
 ***/

void MaterialsManager::lockMaterial( const ResourceID &materialID, UserID newOwner )
{
    LOCK
    materialsOwners_[materialID] = newOwner;
    notifyElementUpdate( materialID );
}


void MaterialsManager::lockMeshMaterials( const ResourceID& meshID, UserID newOwner )
{
    LOCK
    for( const auto& materialID : meshMaterials_.at( meshID ) ){
        lockMaterial( materialID, newOwner );
    }
}


void MaterialsManager::unlockMaterial(const ResourceID &materialID)
{
    LOCK
    materialsOwners_[materialID] = NO_USER;
    notifyElementUpdate( materialID );
}


void MaterialsManager::unlockMeshMaterials(const ResourceID &meshID )
{
    LOCK
    for( const auto& materialID : meshMaterials_.at( meshID ) ){
        unlockMaterial( materialID );
    }
}


void MaterialsManager::unlockUserMaterials( UserID userID )
{
    LOCK
    std::list< ResourceID > meshesToBeUnlocked;

    // All the materials associated to a same mesh have the same owner, so
    // we check the owner of the first material for every mesh and mark
    // the mesh (its materials) to be unlocked if needed.
    // TODO: Change this and iterate over the resources_ map if
    // we allow materials non associated to a mesh.
    for( const auto& meshPair : meshMaterials_ ){
        if( meshPair.second.size() && ( materialsOwners_.at( meshPair.second[0] ) == userID ) ){
            meshesToBeUnlocked.push_back( meshPair.first );
        }
    }

    for( const auto& meshID: meshesToBeUnlocked ){
        unlockMeshMaterials( meshID );
    }
}


/***
 * 11. Materials destruction
 ***/

void MaterialsManager::removeMaterial( const ResourceID &materialID )
{
    LOCK
    materials_.erase( materialID );
    materialsOwners_.erase( materialID );

    notifyElementDeletion( materialID );
}


void MaterialsManager::removeMeshMaterials(const ResourceID &meshID)
{
    LOCK
    for( const auto& materialID : meshMaterials_.at( meshID ) ){
        removeMaterial( materialID );
    }
    meshMaterials_.erase( meshID );
}


void MaterialsManager::removeUserMaterials(UserID userID)
{
    LOCK
    std::list< ResourceID > meshesToBeRemoved;

    // All the materials associated to a same mesh have the same owner, so
    // we check the owner of the first material for every mesh and mark
    // the mesh (its materials) to be deleted if needed.
    // TODO: Change this and iterate over the resources_ map if
    // we allow materials non associated to a mesh.
    for( const auto& meshPair : meshMaterials_ ){
        if( meshPair.second.size() && ( materialsOwners_.at( meshPair.second[0] ) == userID ) ){
            meshesToBeRemoved.push_back( meshPair.first );
        }
    }
    for( const auto& meshID: meshesToBeRemoved ){
        removeMeshMaterials( meshID );
    }
}


} // namespace como
