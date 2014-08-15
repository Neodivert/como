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

#include "transformable.hpp"

namespace como {

/***
 * 4. Transformations
 ***/

void Transformable::translate( glm::vec3 direction )
{
    // Compute the translation matrix.
    const glm::mat4 newTranslation = glm::translate( glm::mat4( 1.0f ), direction );

    applyTransformationMatrix( newTranslation );
}


void Transformable::rotateAroundOrigin( GLfloat angle, glm::vec3 axis )
{
    // Normalize the vector "axis".
    axis = glm::normalize( axis );

    // Compute the rotation matrix for the given angle and the axis converted to
    // object space.
    const glm::mat4 newRotation = glm::rotate( glm::mat4( 1.0f ), angle, axis );
    applyTransformationMatrix( newRotation );
}


void Transformable::rotateAroundCentroid( GLfloat angle, const glm::vec3& axis )
{
    rotateAroundPivot( angle, axis, centroid() );
}


void Transformable::rotateAroundPivot( const GLfloat& angle, const glm::vec3& axis, const glm::vec3& pivot )
{
    translate( -pivot );
    rotateAroundOrigin( angle, axis );
    translate( pivot );
}


void Transformable::scaleAroundPivot( const glm::vec3& scaleFactors, const glm::vec3& pivotPoint )
{
    translate( -pivotPoint );
    scaleAroundOrigin( scaleFactors );
    translate( pivotPoint );
}


void Transformable::scaleAroundOrigin( const glm::vec3& scaleFactors )
{
    // Compute the scale matrix.
    const glm::mat4 newScale = glm::scale( glm::mat4( 1.0f ), scaleFactors );
    applyTransformationMatrix( newScale );
}


void Transformable::scaleAroundCentroid( const glm::vec3& scaleFactors )
{
    scaleAroundPivot( scaleFactors, centroid() );
}


} // namespace como
