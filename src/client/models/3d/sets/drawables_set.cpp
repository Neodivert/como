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

#include "drawables_set.hpp"

namespace como {

/***
 * 1. Construction
 ***/

DrawablesSet::DrawablesSet( const glm::vec4 &borderColor ) :
    borderColor_( borderColor )
{}


/***
 * 3. Getters
 ***/

glm::vec4 DrawablesSet::borderColor() const
{
    return borderColor_;
}


/***
 * 4. Setters
 ***/

void DrawablesSet::setBorderColor( const glm::vec4 &color )
{
    borderColor_ = color;
}

} // namespace como
