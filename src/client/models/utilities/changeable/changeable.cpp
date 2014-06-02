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

#include "changeable.hpp"

namespace como {

/***
 * 1. Construction
 ***/

Changeable::Changeable( bool changed ) :
    changedSinceLastQuery_( changed )
{}


/***
 * 3. Getters
 ***/

bool Changeable::hasChangedSinceLastQuery()
{
    bool hasChanged = changedSinceLastQuery_;

    // We've just queried this object, so set it as "unchanged since last
    // query".
    changedSinceLastQuery_ = false;

    // Return true if this object changed since last query.
    return hasChanged;
}


/***
 * 4. Setters
 ***/

void Changeable::setChanged()
{
    changedSinceLastQuery_ = true;

    onChange();
}

} // namespace como
