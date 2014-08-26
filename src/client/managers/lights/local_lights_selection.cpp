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

#include "local_lights_selection.hpp"

namespace como {

/***
 * 1. Construction
 ***/

LocalLightsSelection::LocalLightsSelection( ServerInterfacePtr server ) :
    EntitiesSet( server->getLocalUserColor().toVec4() ),
    LightsSelection( server->getLocalUserColor().toVec4() ),
    ServerWriter( server )
{
}


/***
 * 3. Lights management
 ***/

ResourceID LocalLightsSelection::addResource( std::unique_ptr<DirectionalLight> resource )
{
    // FIXME: Duplicated code in LocalMeshesSelection::addResource().
    ResourceID resourceID = newResourceID();

    LightsSelection::addResource( resourceID, std::move( resource ) );

    return resourceID;
}


} // namespace como
