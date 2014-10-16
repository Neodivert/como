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

#include "resource_sync_data.hpp"

namespace como {

/***
 * 1. Construction
 ***/

ResourceSyncData::ResourceSyncData( const Command* creationCommand, const ResourceID& id ) :
    SyncData( creationCommand ),
    resourceID_( id ), // TODO: Retrieve ID directly from command
    resourceOwner_( NO_USER )
  // TODO: Initialize name
{}


/***
 * 3. Getters
 ***/

std::list<CommandConstPtr> ResourceSyncData::generateUpdateCommands() const
{
    // TODO: Generate real commands when we allow user to be capable of
    // changing a resource's name.
    return std::list<CommandConstPtr>();
}


ResourceID ResourceSyncData::resourceID() const
{
    return resourceID_;
}


UserID ResourceSyncData::resourceOwner() const
{
    return resourceOwner_;
}

std::list<ResourceID> ResourceSyncData::childResourceIDs() const
{
    return childResourceIDs_;
}


/***
 * 4. Setters
 ***/

void ResourceSyncData::setResourceOwner( UserID newOwner )
{
    resourceOwner_ = newOwner;
}


void ResourceSyncData::addChildResource(const ResourceID &childID)
{
    childResourceIDs_.push_back( childID );
}


/***
 * 4. Updating
 ***/

void ResourceSyncData::processCommand(const Command &command)
{
    (void)( command );
    // TODO: Complete.
}


} // namespace como
