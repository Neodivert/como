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

ResourceID MaterialsManager::createMaterial( const MaterialInfo &materialInfo )
{
    ResourceID materialID = newResourceID();

    // Create the new material and insert it into the materials container.
    createMaterial( materialID, materialInfo );

    return materialID;
}


void MaterialsManager::createMaterials( const ResourceID& meshID, const std::vector< MaterialInfo >& materialsInfo, ResourceID& firstMaterialID )
{
    assert( materialsInfo.size() != 0 );
    ResourceID materialID = firstMaterialID = newResourceID();

    meshMaterials_[meshID] = std::vector<ResourceID>();

    for( auto materialInfo : materialsInfo ){
        createMaterial( materialID, materialInfo );
        meshMaterials_.at( meshID ).push_back( materialID );
        materialID++;
    }
}


void MaterialsManager::createMaterial( ResourceID id, const MaterialInfo& materialInfo )
{
    materials_[id] = MaterialPtr( new Material( materialInfo ) );

    // Set the creator of the material as its current owner.
    lockMaterial( id, id.getCreatorID() );

    notifyElementInsertion( id );
}


/***
 * 4. Material selection
 ***/

MaterialHandlerPtr MaterialsManager::selectMaterial( const ResourceID& id )
{
    if( materialsOwners_.at( id ) != localUserID() ){
        throw std::runtime_error( "You don't have permission for selecting this material!" );
    }

    // TODO: Send command to server.

    // TODO: Remove this when interaction with server is completed.
    if( materialHandler_ ){
        materialHandler_->removeObserver( this );
    }

    materialHandler_ = MaterialHandlerPtr( new MaterialHandler( id, materials_.at( id ), server() ) );

    materialHandler_->addObserver( this );

    //emit materialSelectionConfirmed( materialHandler_ );

    return materialHandler_;
}


/***
 * 5. Getters
 ***/

string MaterialsManager::getResourceName( const ResourceID& resourceID ) const
{
    (void)( resourceID );
    return "Material";
}


MaterialConstPtr MaterialsManager::getMaterial( const ResourceID& id ) const
{
    return materials_.at( id );
}

std::vector<MaterialConstPtr> MaterialsManager::getMaterials( const ResourceID& firstMaterialID, unsigned int nMaterials ) const
{
    std::vector< MaterialConstPtr > materials;
    unsigned int i = 0;
    ResourceID materialID = firstMaterialID;

    materials.reserve( nMaterials );

    for( i = 0; i < nMaterials; i++ ){
        materials.push_back( materials_.at( materialID ) );
        materialID++;
    }

    return materials;
}


/***
 * 6. Remote command execution
 ***/

void MaterialsManager::executeRemoteCommand( MaterialCommandConstPtr command )
{
    //log()->debug( "MaterialsManager - executing remote command - Material ID: ", command->getMaterialID(), "\n" );

    switch( command->getType() ){
        case MaterialCommandType::MATERIAL_CREATION:{
            const MaterialCreationCommand* materialCreationCommand =
                    dynamic_cast< const MaterialCreationCommand* >( command.get() );

            //log()->debug( "\tMaterial name: ", materialCreationCommand->getMaterialInfo().name, "\n" );

            createMaterial( materialCreationCommand->getMaterialID(),
                            materialCreationCommand->getMaterialInfo() );
        }break;

        case MaterialCommandType::MATERIAL_MODIFICATION:{
            const AbstractMaterialModificationCommand* materialModificationCommand =
                    dynamic_cast< const AbstractMaterialModificationCommand* >( command.get() );

            switch( materialModificationCommand->getParameterName() ){
                case MaterialParameterName::COLOR:{
                    const MaterialColorChangeCommand* materialColorChange =
                            dynamic_cast< const MaterialColorChangeCommand* >( materialModificationCommand );

                    materials_.at( command->getMaterialID() )->setColor( materialColorChange->getParameterValue() );
                }break;

                case MaterialParameterName::AMBIENT_REFLECTIVITY:{
                    const MaterialAmbientReflectivityChangeCommand* ambientReflexitivyChange =
                            dynamic_cast< const MaterialAmbientReflectivityChangeCommand* >( materialModificationCommand );

                    materials_.at( command->getMaterialID() )->setAmbientReflectivity( ambientReflexitivyChange->getParameterValue() );
                }break;

                case MaterialParameterName::DIFFUSE_REFLECTIVITY:{
                    const MaterialDiffuseReflectivityChangeCommand* diffuseReflexitivyChange =
                            dynamic_cast< const MaterialDiffuseReflectivityChangeCommand* >( materialModificationCommand );

                    materials_.at( command->getMaterialID() )->setDiffuseReflectivity( diffuseReflexitivyChange->getParameterValue() );
                }break;

                case MaterialParameterName::SPECULAR_REFLECTIVITY:{
                    const MaterialSpecularReflectivityChangeCommand* specularReflexitivyChange =
                            dynamic_cast< const MaterialSpecularReflectivityChangeCommand* >( materialModificationCommand );

                    materials_.at( command->getMaterialID() )->setSpecularReflectivity( specularReflexitivyChange->getParameterValue() );
                }break;

                case MaterialParameterName::SPECULAR_EXPONENT:{
                    const MaterialSpecularExponentChangeCommand* specularExponentChange =
                            dynamic_cast< const MaterialSpecularExponentChangeCommand* >( materialModificationCommand );

                    materials_.at( command->getMaterialID() )->setSpecularExponent( specularExponentChange->getParameterValue() );
                }break;
            }

            //notifyElementModification( command->getMaterialID() );

            notifyObservers();
        }break;
    }
}


void MaterialsManager::update()
{
    //notifyElementModification( materialHandler_->getID() );

    notifyObservers();
}


/***
 * 8. Materials locking
 ***/

void MaterialsManager::lockMaterial( const ResourceID &materialID, UserID newOwner )
{
    materialsOwners_[materialID] = newOwner;
    notifyElementUpdate( materialID );
}


void MaterialsManager::lockMeshMaterials( const ResourceID& meshID, UserID newOwner )
{
    for( const auto& materialID : meshMaterials_.at( meshID ) ){
        lockMaterial( materialID, newOwner );
    }
}


void MaterialsManager::unlockMaterial(const ResourceID &materialID)
{
    materialsOwners_[materialID] = NO_USER;
    notifyElementUpdate( materialID );
}


void MaterialsManager::unlockMeshMaterials(const ResourceID &meshID )
{
    for( const auto& materialID : meshMaterials_.at( meshID ) ){
        unlockMaterial( materialID );
    }
}


void MaterialsManager::unlockUserMaterials( UserID userID )
{
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
 * 9. Materials destruction
 ***/

void MaterialsManager::removeMaterial( const ResourceID &materialID )
{
    materials_.erase( materialID );
    materialsOwners_.erase( materialID );

    notifyElementDeletion( materialID );
}


void MaterialsManager::removeMeshMaterials(const ResourceID &meshID)
{
    for( const auto& materialID : meshMaterials_.at( meshID ) ){
        removeMaterial( materialID );
    }
    meshMaterials_.erase( meshID );
}


void MaterialsManager::removeUserMaterials(UserID userID)
{
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
