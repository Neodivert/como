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

ResourcesOwnershipManager::ResourcesOwnershipManager( UsersMap& usersMap, LogPtr log ) :
    users_( usersMap ),
    log_( log )
{}


/***
 * 3. Resources registation
 ***/

void ResourcesOwnershipManager::registerResource(const ResourceID& resourceID, UserID ownerID )
{
    resourcesOwnershipMap_[ resourceID ] = ownerID;
}


/***
 * 5. Resources ownership management
 ***/

void ResourcesOwnershipManager::lockResource( const ResourceID& resourceID, UserID userID )
{
    log_->debug( "User (", userID, ") locks resource (", resourceID, "): " );
    if( resourcesOwnershipMap_.at( resourceID ) == NO_USER ){
        resourcesOwnershipMap_.at( resourceID ) = userID;
        users_.at( userID )->addSelectionResponse( true );
        log_->debug( "Yes!\n" );
    }else{
        users_.at( userID )->addSelectionResponse( false );
        log_->debug( "No, resource already locked! :'-(\n" );
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


void ResourcesOwnershipManager::processLockResponse( const ResourceID& resourceID, bool lockResponse )
{
    // TODO: Make this trick unnecessary.
    (void)( resourceID );
    (void)( lockResponse );
    throw std::runtime_error( "ResourcesOwnershipManager::processLockResponse called!" );
}

} // namespace como
