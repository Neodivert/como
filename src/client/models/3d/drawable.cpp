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

#include "drawable.hpp"
#include <stdexcept>

namespace como {

std::string getOpenGLErrorString( GLenum errorCode )
{
    switch( errorCode ){
        case GL_NO_ERROR:
            return std::string( "No error" );
        break;
        case GL_INVALID_ENUM:
            return std::string( "Invalid enum" );
        break;
        case GL_INVALID_VALUE:
            return std::string( "Invalid value" );
        break;
        case GL_INVALID_OPERATION:
            return std::string( "Invalid operation" );
        break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return std::string( "Invalid framebuffer operation" );
        break;
        case GL_OUT_OF_MEMORY:
            return std::string( "Out of memory" );
        break;
        case GL_STACK_UNDERFLOW:
            return std::string( "Stack underflow" );
        break;
        case GL_STACK_OVERFLOW:
            return std::string( "Stack overflow" );
        break;
        default:
            return std::string( "Unknown error" );
        break;
    }
}


void checkOpenGL( const char* str )
{
    GLenum errorCode = glGetError();

    if( errorCode != GL_NO_ERROR ){
        throw std::runtime_error( std::string( "OpenGL ERROR at [" ) +
                                  std::string( str ) +
                                  std::string( "]: " ) +
                                  getOpenGLErrorString( errorCode ) +
                                  std::string( "\n" )
                                  );
    }
}


/***
 * 1. Initialization and destruction
 ***/

Drawable::Drawable( DrawableType type, std::string name ) :
    type_( type ),
    name_( name )
{
    // Initialize transformation matrixes to identity matrix.
    transformationMatrix = glm::mat4( 1.0f );

    // Initialize the drawable's original orientation.
    originalOrientation[X] = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
    originalOrientation[Y] = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
    originalOrientation[Z] = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );
}


/***
 * 2. Getters
 ***/

glm::mat4 Drawable::getTransformationMatrix()
{
    return transformationMatrix;
}


DrawableType Drawable::getType() const
{
    return type_;
}


std::string Drawable::getName() const
{
    return name_;
}


/***
 * 3. Transformations
 ***/

void Drawable::translate( glm::vec3 direction )
{
    // Compute the translation matrix.
    glm::mat4 newTranslation = glm::translate( glm::mat4( 1.0f ), direction );

    // Move the drawable from object to world space, then apply the translation and
    // finally move back the drawable from world to object space.
    transformationMatrix = transformationMatrix * glm::inverse( transformationMatrix ) * newTranslation * transformationMatrix;

    // Update the transformed vertices using the original ones and the
    // previous transformation matrix.
    update();
}

#include <cstdlib>

void Drawable::rotate( GLfloat angle, glm::vec3 axis )
{
    // Normalize the vector "axis".
    axis = glm::normalize( axis );

    // Compute the rotation matrix for the given angle and the axis converted to
    // object space.
    glm::mat4 newRotation = glm::rotate( glm::mat4( 1.0f ), angle, axis );

    // Move the drawable from object to world space, then apply the rotation and
    // finally move back the drawable from world to object space.
    transformationMatrix = transformationMatrix * glm::inverse( transformationMatrix ) * newRotation * transformationMatrix;

    // Update the transformed vertices using the original ones and the
    // previous transformation matrix.
    update();
}


void Drawable::rotate( const GLfloat& angle, glm::vec3 axis, const glm::vec3& pivot )
{
    translate( -pivot );
    rotate( angle, axis );
    translate( pivot );
    /*
    // Normalize the vector "axis".
    axis = glm::normalize( axis );

    // Compute the rotation matrix for the given angle and the axis converted to
    // object space.
    glm::mat4 newRotation = glm::rotate( glm::mat4( 1.0f ), angle, axis );

    // Compute the translation matrix for moving the drawable to the pivot point.
    //glm::mat4 pivotTranslation = glm::translate( glm::mat4( 1.0f ), -pivot );

    // Move the drawable from object to world space, then apply the rotation
    // around the pivot point and finally move back the drawable from world
    // to object space.
    translate( -pivot );
    transformationMatrix = transformationMatrix * glm::inverse( transformationMatrix ) * newRotation * transformationMatrix;
    translate( pivot );

    // Update the transformed vertices using the original ones and the
    // previous transformation matrix.
    update();
    */
}


void Drawable::scale( glm::vec3 scaleFactors )
{
    // Compute the scale matrix.
    glm::mat4 newScale = glm::scale( glm::mat4( 1.0f ), scaleFactors );

    // Move the drawable from object to world space, then apply the scale and
    // finally move back the drawable from world to object space.
    transformationMatrix = transformationMatrix * glm::inverse( transformationMatrix ) * newScale * transformationMatrix;

    // Update the transformed vertices using the original ones and the
    // previous transformation matrix.
    update();
}

void Drawable::scale( glm::vec3 scaleFactors, const glm::vec3& pivotPoint )
{
    translate( -pivotPoint );
    scale( scaleFactors );
    translate( pivotPoint );
}


/***
 * 6. Updating and drawing
 ***/

void Drawable::update()
{
    //transformationMatrix = rotationMatrix * translationMatrix;

    for( unsigned int i = 0; i<3; i++ ){
        transformedOrientation[i] = transformationMatrix * originalOrientation[i];
    }
}


/***
 * 7. Auxiliar methods
 ***/

glm::vec4 Drawable::transformScaleVector( glm::vec4 scaleVector, const glm::mat4& transformMatrix )
{
    glm::vec4 reversalVector;

    // If a coordinate has been inverted, indicate it in the reversal vector.
    if( scaleVector.x < 0.0f ){
        reversalVector.x = 1.0f;
    }
    if( scaleVector.y < 0.0f ){
        reversalVector.y = 1.0f;
    }

    // Transform the scale and the reversal vectors with the given transform matrix.
    scaleVector = glm::inverse( transformMatrix ) * scaleVector;
    reversalVector = glm::inverse( transformMatrix ) * reversalVector;

    // When the transformation vector is moved from window to world space, some
    // components can be inverted due to the rotations inherent to the space change.
    // Because of that, we use the transformed reversal vector in order to know when a reversion
    // is "true" (commanded by user) or "false" (due to space change).
    for( unsigned int i=0; i<3; i++ ){
        if( abs( reversalVector[i] ) > 0.00001f ){
            scaleVector[i] = - abs( scaleVector[i] );
        }else{
            scaleVector[i] = abs( scaleVector[i] );
        }
    }

    return scaleVector;
}


} // namespace como.
