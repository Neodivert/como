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

#include "lights_manager.hpp"

namespace como {


/***
 * 1. Construction
 ***/

LightsManager::LightsManager( unsigned int maxDirectionalLights,
                              ResourcesOwnershipManager *resourcesOwnershipManager ) :
    MAX_LIGHTS( maxDirectionalLights ),
    MAX_DIRECTIONAL_LIGHTS( maxDirectionalLights ),
    resourcesOwnershipManager_( resourcesOwnershipManager )
{
    resourcesOwnershipManager_->addObserver( this );
}


/***
 * 3. Updating (Observer pattern)
 ***/

void LightsManager::update( ContainerAction lastContainerAction, ResourceID lastElementModified )
{
    lock();
    if( ( lastContainerAction == ContainerAction::ELEMENT_DELETION )
            && lights_.count( lastElementModified )  ){
        lights_.erase( lastElementModified );
    }
}


/***
 * 4. Lights management
 ***/

bool LightsManager::requestDirectionalLightCreation( const ResourceID& lightID )
{
    lock();
    if( lights_.size() < MAX_DIRECTIONAL_LIGHTS ){
        lights_.insert( lightID );
        resourcesOwnershipManager_->registerResource( lightID, lightID.getCreatorID() );
        return true;
    }else{
        return false;
    }
}

} // namespace como
