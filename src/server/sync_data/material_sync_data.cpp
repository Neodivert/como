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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "material_sync_data.hpp"
#include <common/commands/material_commands/material_modification_command.hpp>

namespace como {

/***
 * 1. Construction
 ***/

MaterialSyncData::MaterialSyncData( const Command *creationCommand, const ResourceID &id ) :
    ResourceSyncData( creationCommand, id )
{}


/***
 * 3. Getters
 ***/

std::list<CommandConstPtr> MaterialSyncData::generateUpdateCommands() const
{
    std::list<CommandConstPtr> updateCommands =
            ResourceSyncData::generateUpdateCommands();

    updateCommands.push_back(
                CommandConstPtr(
                    new MaterialColorChangeCommand(
                        NO_USER,
                        resourceID(),
                        material_.color ) ) );

    updateCommands.push_back(
                CommandConstPtr(
                    new MaterialAmbientReflectivityChangeCommand(
                        NO_USER,
                        resourceID(),
                        material_.ambientReflectivity ) ) );

    updateCommands.push_back(
                CommandConstPtr(
                    new MaterialDiffuseReflectivityChangeCommand(
                        NO_USER,
                        resourceID(),
                        material_.diffuseReflectivity ) ) );

    updateCommands.push_back(
                CommandConstPtr(
                    new MaterialSpecularReflectivityChangeCommand(
                        NO_USER,
                        resourceID(),
                        material_.specularReflectivity ) ) );

    updateCommands.push_back(
                CommandConstPtr(
                    new MaterialSpecularExponentChangeCommand(
                        NO_USER,
                        resourceID(),
                        material_.specularExponent ) ) );

    return updateCommands;
}


/***
 * 4. Updating
 ***/

void MaterialSyncData::processCommand(const Command &command)
{
    if( command.getTarget() == CommandTarget::MATERIAL ){
        const MaterialCommand& materialCommand =
                dynamic_cast< const MaterialCommand& >( command );

        switch( materialCommand.getType() ){
            case MaterialCommandType::MATERIAL_CREATION:{
                throw std::runtime_error( "MATERIAL_CREATION command received" );
            }break;
            case MaterialCommandType::MATERIAL_MODIFICATION:{
                const AbstractMaterialModificationCommand& materialModificationCommand =
                        dynamic_cast< const AbstractMaterialModificationCommand& >( materialCommand );

                switch( materialModificationCommand.getParameterName() ){
                    case MaterialParameterName::COLOR:{
                        const MaterialColorChangeCommand& materialColorChange =
                                dynamic_cast< const MaterialColorChangeCommand& >( materialModificationCommand );

                        material_.color = materialColorChange.getParameterValue().toVec4();
                    }break;

                    case MaterialParameterName::AMBIENT_REFLECTIVITY:{
                        const MaterialAmbientReflectivityChangeCommand& ambientReflexitivyChange =
                                dynamic_cast< const MaterialAmbientReflectivityChangeCommand& >( materialModificationCommand );

                        material_.ambientReflectivity = ambientReflexitivyChange.getParameterValue().toVec3();
                    }break;

                    case MaterialParameterName::DIFFUSE_REFLECTIVITY:{
                        const MaterialDiffuseReflectivityChangeCommand& diffuseReflexitivyChange =
                                dynamic_cast< const MaterialDiffuseReflectivityChangeCommand& >( materialModificationCommand );

                        material_.diffuseReflectivity = diffuseReflexitivyChange.getParameterValue().toVec3();
                    }break;

                    case MaterialParameterName::SPECULAR_REFLECTIVITY:{
                        const MaterialSpecularReflectivityChangeCommand& specularReflexitivyChange =
                                dynamic_cast< const MaterialSpecularReflectivityChangeCommand& >( materialModificationCommand );

                        material_.specularReflectivity = specularReflexitivyChange.getParameterValue().toVec3();
                    }break;

                    case MaterialParameterName::SPECULAR_EXPONENT:{
                        const MaterialSpecularExponentChangeCommand& specularExponentChange =
                                dynamic_cast< const MaterialSpecularExponentChangeCommand& >( materialModificationCommand );

                        material_.specularExponent = specularExponentChange.getParameterValue();
                    }break;
                }
            }break;
        }
    }
}



} // namespace como
