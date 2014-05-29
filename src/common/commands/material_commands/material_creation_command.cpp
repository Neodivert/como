#include "material_creation_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

MaterialCreationCommand::MaterialCreationCommand() :
    MaterialCommand( MaterialCommandType::MATERIAL_CREATION )
{}

MaterialCreationCommand::MaterialCreationCommand( const MaterialID& materialID ) :
    MaterialCommand( MaterialCommandType::MATERIAL_CREATION, materialID )
{}

MaterialCreationCommand::MaterialCreationCommand(MaterialCreationCommand &b) :
    MaterialCommand( b )
{}

}
