#include "abstract_entities_manager.hpp"

namespace como {

/***
 * 1. Construction
 ***/

AbstractEntitiesManager::AbstractEntitiesManager( ServerInterfacePtr server ) :
    ResourceCommandsExecuter( server )
{}


/***
 * 4. Selecting
 ***/

ResourceID AbstractEntitiesManager::selectEntityByRayPicking(glm::vec3 rayOrigin, glm::vec3 rayDirection, bool addToSelection, float &t, const float &MAX_T)
{
    LOCK
    ResourceID closestObject = NO_RESOURCE;
    // Does the user want to keep the actual set of selected objects and simply add
    // a new one? If that's NOT the case, we need to clear the set of selected drawables
    // first.
    if( !addToSelection ){
        requestSelectionUnlock();
    }

    if( pick( rayOrigin, rayDirection, closestObject, t, MAX_T ) ){
        // Request to the server the lock of the intersected drawable.
        requestResourceLock( closestObject );
    }

    return closestObject;
}

} // namespace como
