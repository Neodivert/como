#include "material_creation_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

MaterialCreationCommand::MaterialCreationCommand() :
    MaterialCommand( MaterialCommandType::MATERIAL_CREATION ),
    materialName_( "Unnamed material" )
{
    addPackable( &materialName_ );
}

MaterialCreationCommand::MaterialCreationCommand( const MaterialID& materialID, const std::string& materialName ) :
    MaterialCommand( MaterialCommandType::MATERIAL_CREATION, materialID ),
    materialName_( materialName.c_str() )
{
    addPackable( &materialName_ );
}

MaterialCreationCommand::MaterialCreationCommand( MaterialCreationCommand &b ) :
    MaterialCommand( b ),
    materialName_( b.materialName_ )
{
    addPackable( &materialName_ );
}

/***
 * 3. Getters
 ***/

std::string MaterialCreationCommand::getMaterialName() const
{
    return materialName_.getValue();
}

}
