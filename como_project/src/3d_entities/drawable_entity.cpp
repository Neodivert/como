/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "drawable_entity.hpp"

/***
 * 1. Initialization
 ***/

DrawableEntity::DrawableEntity()
{
    // Initialize transformation matrix to identity matrix.
    transformationMatrix = glm::mat4( 1.0f );
}


/***
 * 2. Transformations
 ***/

void DrawableEntity::translate( const GLfloat& tx, const GLfloat& ty, const GLfloat& tz )
{
    // Multiply the drawable's transformation matrix by a translation one.
    transformationMatrix = glm::translate( transformationMatrix, glm::vec3( tx, ty, tz ) );

    // Update the transformed vertices using the original ones and the
    // previous transformation matrix.
    update();
}


void rotate( const GLfloat& angle, const GLfloat& x, const GLfloat& y, const GLfloat& z )
{
    // Multiply the drawable's transformation matrix by a rotation one.
    transformationMatrix = glm::rotate( transformationMatrix, angle, glm::vec3( x, y, z ) );

    // Update the transformed vertices using the original ones and the
    // previous transformation matrix.
    update();
}
