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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "resources_manager.hpp"

namespace como {


/***
 * 1. Construction
 ***/

ResourcesManager::ResourcesManager( ServerInterfacePtr server ) :
    AbstractResourcesOwnershipManager(),
    server_( server )
{}


/***
 * 4. Server communication
 ***/

void ResourcesManager::sendCommandToServer( CommandConstPtr command )
{
    server_->sendCommand( command );
}


/***
 * 5. Server info
 ***/

UserID ResourcesManager::localUserID() const
{
    return server_->getLocalUserID();
}


ResourceID ResourcesManager::newResourceID()
{
    return server_->getNewResourceID();
}


ServerInterfacePtr ResourcesManager::server() const
{
    return server_;
}


} // namespace como
