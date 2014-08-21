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
#include <boost/lexical_cast.hpp>

namespace como {

/***
 * 1. Creation
 ***/

MaterialsManager::MaterialsManager( ServerInterfacePtr server, LogPtr log ) :
    ResourcesManager( server, log )
{}


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


void MaterialsManager::createMaterials( const std::vector< MaterialInfo >& materialsInfo, ResourceID& firstMaterialID )
{
    assert( materialsInfo.size() != 0 );
    ResourceID materialID = firstMaterialID = newResourceID();

    for( auto materialInfo : materialsInfo ){
        createMaterial( materialID, materialInfo );
        materialID++;
    }
}

void MaterialsManager::createMaterial( ResourceID id, const MaterialInfo& materialInfo )
{
    materials_[id] = MaterialPtr( new Material( materialInfo ) );

    // Set the creator of the material as its current owner.
    materialsOwners_[id] = id.getCreatorID();

    //notifyElementInsertion( id );
}


void MaterialsManager::createRemoteMaterials( const std::vector< MaterialInfo >& materialsInfo, const ResourceID& firstMaterialID )
{
    assert( materialsInfo.size() != 0 );

    ResourceID materialID = firstMaterialID;

    for( auto materialInfo : materialsInfo ){
        createMaterial( materialID, materialInfo );
        materialID++;
    }
}


/***
 * 4. Material selection
 ***/

void MaterialsManager::selectMaterial( const ResourceID& id )
{
    MaterialsManager::selectMaterial( localUserID(), id );

    // TODO: Send command to server.

    // TODO: Remove this when interaction with server is completed.
    if( materialHandler_ ){
        materialHandler_->removeObserver( this );
    }

    materialHandler_ = MaterialHandlerPtr( new MaterialHandler( id, materials_.at( id ), server() ) );

    materialHandler_->addObserver( this );

    emit materialSelectionConfirmed( materialHandler_ );
}


void MaterialsManager::selectMaterial( UserID userID, const ResourceID& id )
{
    Q_UNUSED( userID );
    Q_UNUSED( id );

    // TODO: Complete this method.
}


/***
 * 5. Getters
 ***/

MaterialHandlerPtr MaterialsManager::getCurrentMaterial() const
{
    return materialHandler_;
}


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
 * 9. Auxiliar methods
 ***/

void MaterialsManager::highlightMaterial( ResourceID materialID )
{
    (void)( materialID );
    //drawablesManager_->highlightProperty( getMaterial( materialID ).get() );
}


void MaterialsManager::removeHighlights()
{
    //drawablesManager_->highlightProperty( nullptr );
}


/***
 * 10. Resources management
 ***/

void MaterialsManager::lockResource( const ResourceID& resourceID, UserID userID )
{
    (void)( resourceID );
    (void)( userID );
    // TODO: Complete.
}


void MaterialsManager::unlockResourcesSelection( UserID userID )
{
    (void)( userID );
    // TODO: Complete.
}


void MaterialsManager::deleteResourcesSelection( UserID userID )
{
    (void)( userID );
    // TODO: Complete.
}

} // namespace como
