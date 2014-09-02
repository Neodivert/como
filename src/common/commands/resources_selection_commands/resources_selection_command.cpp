#include "resources_selection_command.hpp"

namespace como {


/***
 * 1. Construction
 ***/

ResourcesSelectionCommand::ResourcesSelectionCommand( ResourcesSelectionCommandType commandType ) :
    TypeCommand( CommandTarget::RESOURCES_SELECTION, commandType, NO_USER )
{}


ResourcesSelectionCommand::ResourcesSelectionCommand( ResourcesSelectionCommandType commandType, UserID userID ) :
    TypeCommand( CommandTarget::RESOURCES_SELECTION, commandType, userID )
{}


} // namespace como
