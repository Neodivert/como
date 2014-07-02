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

MaterialsManager::MaterialsManager( UserID localUserID, ServerInterfacePtr server, LogPtr log ) :
    nextLocalMaterialID_( localUserID, 0 ),
    localUserID_( nextLocalMaterialID_.getCreatorID() ),
    server_( server ),
    log_( log )
{}


/***
 * 3. Material creation
 ***/

MaterialID MaterialsManager::createMaterial( const MaterialInfo &materialInfo )
{
    // Create the new material and insert it into the materials container.
    createMaterial( nextLocalMaterialID_, materialInfo );

    return nextLocalMaterialID_++;
}


void MaterialsManager::createMaterial( MaterialID materialID, const MaterialInfo& materialInfo )
{
    // Create the new material and insert it into the materials container.
    materials_[materialID] = MaterialPtr( new Material( materialInfo ) );

    // Set the creator of the material as its current owner.
    materialsOwners_[materialID] = materialID.getCreatorID();

    notifyElementInsertion( materialID );
}


/***
 * 4. Material selection
 ***/

void MaterialsManager::selectMaterial( const MaterialID& id )
{
    MaterialsManager::selectMaterial( localUserID_, id );

    // TODO: Send command to server.

    // TODO: Remove this when interaction with server is completed.
    if( materialHandler_ ){
        materialHandler_->removeObserver( this );
    }

    materialHandler_ = MaterialHandlerPtr( new MaterialHandler( id, materials_.at( id ), server_ ) );

    materialHandler_->addObserver( this );

    emit materialSelectionConfirmed( materialHandler_ );
}


void MaterialsManager::selectMaterial( UserID userID, const MaterialID& id )
{
    Q_UNUSED( userID );
    Q_UNUSED( id );

    // TODO: Complete this method.
}


/***
 * 5. Getters
 ***/

MaterialConstPtr MaterialsManager::getMaterial( const MaterialID& id ) const
{
    return materials_.at( id );
}


/***
 * 6. Remote command execution
 ***/

void MaterialsManager::executeRemoteCommand( MaterialCommandConstPtr command )
{
    log_->debug( "MaterialsManager - executing remote command - Material ID: ", command->getMaterialID(), "\n" );

    switch( command->getType() ){
        case MaterialCommandType::MATERIAL_CREATION:{
            const MaterialCreationCommand* materialCreationCommand =
                    dynamic_cast< const MaterialCreationCommand* >( command.get() );

            log_->debug( "\tMaterial name: ", materialCreationCommand->getMaterialInfo().name, "\n" );

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

            notifyElementModification( command->getMaterialID() );

            setChanged();
        }break;
    }
}


void MaterialsManager::update()
{
    notifyElementModification( materialHandler_->getID() );

    setChanged();
}


} // namespace como
