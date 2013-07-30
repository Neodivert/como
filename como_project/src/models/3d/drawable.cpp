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

/***
 * 1. Initialization
 ***/

namespace como {

GLint Drawable::mvpMatrixLocation = -1;

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
    GLint currentShaderProgram;

    // Initialize transformation matrix to identity matrix.
    transformationMatrix = glm::mat4( 1.0f );

    // Initialize the drawable's original orientation.
    originalOrientation[X] = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
    originalOrientation[Y] = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
    originalOrientation[Z] = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );

    // Get location of uniform shader modelview matrix.
    if( mvpMatrixLocation == -1 ){
        // Get current shader program id.
        glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

        mvpMatrixLocation = glGetUniformLocation( currentShaderProgram, "mvpMatrix" );
        cout << "mvpMatrixLocation: (" << mvpMatrixLocation << ")" << endl;
    }
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

void Drawable::translate( const glm::vec3& direction )
{
    // Multiply the drawable's transformation matrix by a translation one.
    transformationMatrix = glm::translate( transformationMatrix, direction );

    // Update the transformed vertices using the original ones and the
    // previous transformation matrix.
    update();
}


void Drawable::rotate( const GLfloat& angle, const glm::vec3& axis )
{
    // Multiply the drawable's transformation matrix by a rotation one.
    transformationMatrix = glm::rotate( transformationMatrix, angle, axis );

    // Update the transformed vertices using the original ones and the
    // previous transformation matrix.
    update();
}


void Drawable::sendMvpMatrixToShader( const glm::mat4& mvpMatrix ) const
{
    glUniformMatrix4fv( mvpMatrixLocation, 1, GL_FALSE, &mvpMatrix[0][0] );
}


/***
 * 5. Updating and drawing
 ***/

void Drawable::update()
{
    for( unsigned int i = 0; i<3; i++ ){
        transformedOrientation[i] = transformationMatrix * originalOrientation[i];
    }
}


} // namespace como.
