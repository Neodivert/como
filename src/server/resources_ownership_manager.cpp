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

#include "resources_ownership_manager.hpp"

namespace como {


/***
 * 1. Construction
 ***/

ResourcesOwnershipManager::ResourcesOwnershipManager( UsersMap& usersMap ) :
    users_( usersMap )
{}


/***
 * 4. Resources ownership management
 ***/

void ResourcesOwnershipManager::lockResource( const ResourceID& resourceID, UserID userID )
{
    if( resourcesOwnershipMap_.at( resourceID ) == NO_USER ){
        resourcesOwnershipMap_.at( resourceID ) = userID;
        users_.at( userID )->addSelectionResponse( true );
    }else{
        users_.at( userID )->addSelectionResponse( false );
    }
}

void ResourcesOwnershipManager::unlockResourcesSelection( UserID userID )
{
    for( auto resourceOwnership : resourcesOwnershipMap_ ){
        if( resourceOwnership.second == userID ){
            resourceOwnership.second = NO_USER;
        }
    }
}

void ResourcesOwnershipManager::deleteResourcesSelection( UserID userID )
{
    ResourcesOwnershipMap::iterator currentElement, nextElement;

    currentElement = nextElement = resourcesOwnershipMap_.begin();
    while( currentElement != resourcesOwnershipMap_.end() ){
        nextElement++;

        if( currentElement->second == userID ){
            resourcesOwnershipMap_.erase( currentElement );
        }

        currentElement = nextElement;
    }
}

} // namespace como
