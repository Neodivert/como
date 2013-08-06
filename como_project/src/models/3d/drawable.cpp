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

#include "drawable.hpp"
#include <glm/gtx/vector_angle.hpp>

/***
 * 1. Initialization
 ***/

namespace como {

void showError(){
    GLenum errCode;
    if ((errCode = glGetError()) != GL_NO_ERROR)
    {
        cout << "ERROR: " << gluErrorString(errCode) << endl;
    }/*else{
        cout << "NO ERROR" << endl;
    }*/
}


Drawable::Drawable()
{
    // Initialize transformation matrix to identity matrix.
    translationMatrix = glm::mat4( 1.0f );
    rotationMatrix = glm::mat4( 1.0f );
    transformationMatrix = glm::mat4( 1.0f );

    // Initialize the drawable's original orientation.
    originalOrientation[X] = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
    originalOrientation[Y] = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
    originalOrientation[Z] = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );
}


/***
* 2. Setters and getters
 ***/

glm::mat4 Drawable::getTransformationMatrix()
{
    return transformationMatrix;
}


/***
 * 3. Transformations
 ***/

glm::vec3 Drawable::transformToObjectSpace( const glm::vec3 worldVector )
{
    return glm::vec3( glm::inverse( rotationMatrix ) * glm::vec4( worldVector, 1.0f ) );
    //glm::vec4 dir =
}

void Drawable::translate( glm::vec3 direction )
{
    // Rotate the vector "direction" from world to object space.
    direction = glm::vec3( glm::inverse( rotationMatrix ) * glm::vec4( direction, 1.0f ) );

    // Add translation to transformation matrix.
    transformationMatrix = glm::translate( transformationMatrix, direction );

    // Update the transformed vertices using the original ones and the
    // previous transformation matrix.
    update();
}

#include <cstdlib>

void Drawable::rotate( GLfloat angle, glm::vec3 axis )
{
    /*
    const glm::vec3 yAxis( 0.0f, 1.0f, 0.0f );

    cout << "rotate()" << endl
         << "Angle: " << angle << endl
         << "Axis (world space): (" << axis.x << ", " << axis.y << ", " << axis.z << ")" << endl;
    */

    // Rotate rotation's axis from world to object space.
    axis = glm::normalize( glm::vec3( glm::inverse( rotationMatrix ) * glm::vec4( axis, 1.0f ) ) );
    //cout << "Axis (object space): (" << axis.x << ", " << axis.y << ", " << axis.z << ")" << endl;

    // Compute the rotation matrix for the given angle and the axis converted to
    // object space.
    glm::mat4 newRotation = glm::rotate( glm::mat4( 1.0f ), angle, axis );

    // Concatenate new rotation to previous rotations.
    rotationMatrix = rotationMatrix * newRotation;

    // Concatenate new rotation to object's transformation matrix.
    transformationMatrix = transformationMatrix * newRotation;

    /*
    glm::vec3 up = glm::normalize( glm::vec3( transformationMatrix * glm::vec4( yAxis, 1.0f ) ) );

    cout << "New up: (" << up.x << ", " << up.y << ", " << up.z << ")" << endl;
    cout << "Angle between up and global Y vectors: " << glm::angle( up, yAxis ) << endl;
    */

    // Update the transformed vertices using the original ones and the
    // previous transformation matrix.
    update();
}


void Drawable::rotate( const GLfloat& angle, const glm::vec3& axis, const glm::vec3& pivot )
{
    cout << "Rotating around pivot: (" << pivot.x << ", " << pivot.y << ", " << pivot.z << ")" << endl;

    //glm::mat4 rotationMatrix = glm::rotate(  );
    // Translate drawable until the pivot be at the origin.
    //transformationMatrix = glm::translate( transformationMatrix, -pivot );

    /*
    // Rotate drawable.
    glm::mat4 rotationMatrix = glm::rotate( glm::mat4( 1.0f ), angle, axis );

    transformationMatrix = rotationMatrix * transformationMatrix;
    //transformationMatrix = glm::rotate( transformationMatrix, angle, axis );

    // Translate drawable until its original position.
    //transformationMatrix = glm::translate( transformationMatrix, pivot );

    // Update the transformed vertices using the original ones and the
    // previous transformation matrix.
    update();
    */
}


/***
 * 5. Updating and drawing
 ***/

void Drawable::update()
{
    //transformationMatrix = rotationMatrix * translationMatrix;

    for( unsigned int i = 0; i<3; i++ ){
        transformedOrientation[i] = transformationMatrix * originalOrientation[i];
    }
}


} // namespace como.
