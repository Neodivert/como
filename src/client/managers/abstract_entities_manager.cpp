#include "abstract_entities_manager.hpp"

namespace como {

/***
 * 1. Construction
 ***/

AbstractEntitiesManager::AbstractEntitiesManager( ServerInterfacePtr server ) :
    ResourceCommandsExecuter( server )
{}

} // namespace como
