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

DrawableEntity::DrawableEntity()
{
    // Initialize transformation matrix to identity matrix.
    transformationMatrix = glm::mat4( 1.0f );
}



void DrawableEntity::translate( const GLfloat& tx, const GLfloat& ty, const GLfloat& tz )
{
    cout << "Translating" << endl;
    transformationMatrix = glm::translate( transformationMatrix, glm::vec3( tx, ty, tz ) );
    update();
}

