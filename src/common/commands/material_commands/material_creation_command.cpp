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

MaterialCreationCommand::MaterialCreationCommand( const MaterialID& materialID, const std::string& materialName ) :
    MaterialCommand( MaterialCommandType::MATERIAL_CREATION, materialID ),
    materialName_( materialName.c_str() )
{
    ADD_PACKABLES
}

MaterialCreationCommand::MaterialCreationCommand( MaterialCreationCommand &b ) :
    MaterialCommand( b ),
    materialName_( b.materialName_ )
{
    ADD_PACKABLES
}



/***
 * 3. Getters
 ***/

MaterialInfo MaterialCreationCommand::getMaterialInfo() const
{
    MaterialInfo materialInfo;

    materialInfo.name = materialName_.getValue();
    materialInfo.ambientReflectivity = materialAmbientReflectivity_.toVec3();
    materialInfo.diffuseReflectivity = materialDiffuseReflectivity_.toVec3();
    materialInfo.specularReflectivity = materialSpecularReflectivity_.toVec3();
    materialInfo.specularExponent = materialSpecularExponent_.getValue();

    return materialInfo;
}

}
