#include "material_creation_command.hpp"

namespace como {

#define ADD_PACKABLES \
    addPackable( &materialName_ ); \
    addPackable( &materialAmbientReflectivity_ ); \
    addPackable( &materialDiffuseReflectivity_ ); \
    addPackable( &materialSpecularReflectivity_ ); \
    addPackable( &materialSpecularExponent_ ); \

/***
 * 1. Construction
 ***/

MaterialCreationCommand::MaterialCreationCommand() :
    MaterialCommand( MaterialCommandType::MATERIAL_CREATION ),
    materialName_( "Unnamed material" )
{
    ADD_PACKABLES
}

MaterialCreationCommand::MaterialCreationCommand( const ResourceID& materialID, const std::string& materialName ) :
    MaterialCommand( MaterialCommandType::MATERIAL_CREATION, materialID.getCreatorID(), materialID ),
    materialName_( materialName.c_str() )
{
    ADD_PACKABLES
}

MaterialCreationCommand::MaterialCreationCommand( const MaterialCreationCommand &b ) :
    MaterialCommand( b ),
    materialName_( b.materialName_ )
{
    ADD_PACKABLES
}



/***
 * 3. Getters
 ***/

MaterialInfo MaterialCreationCommand::getMaterialData() const
{
    MaterialInfo materialInfo;

    materialInfo.name = materialName_.getValue();
    materialInfo.ambientReflectivity = materialAmbientReflectivity_.getValue().toVec3();
    materialInfo.diffuseReflectivity = materialDiffuseReflectivity_.getValue().toVec3();
    materialInfo.specularReflectivity = materialSpecularReflectivity_.getValue().toVec3();
    materialInfo.specularExponent = materialSpecularExponent_.getValue();

    return materialInfo;
}

}
