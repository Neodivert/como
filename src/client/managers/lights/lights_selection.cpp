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

#include "lights_selection.hpp"

namespace como {

/***
 * 1. Construction
 ***/

LightsSelection::LightsSelection( DrawablesSelectionPtr drawablesSelection ) :
    drawablesSelection_( drawablesSelection )
{
    drawablesSelection->addObserver( this );
}


/***
 * 2. Destruction
 ***/

LightsSelection::~LightsSelection()
{
    drawablesSelection_->removeObserver( this );
}


/***
 * 3. Updating (Observer pattern).
 ***/

void LightsSelection::update( ContainerAction lastContainerAction, ResourceID lastElementModified )
{
    if( lastContainerAction == ContainerAction::ELEMENT_DELETION ){
        lightsProperties_.erase( lastElementModified );
    }
}

} // namespace como
