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

#include "selection_transformation.hpp"
#include <cstdio>
#include <stdexcept>

namespace como {

//const char FLOAT_TO_STRING_FORMAT[32] = "%.3f#%.3f#%.3f";
const char FLOAT_TO_STRING_FORMAT[32] = "%f#%f#%f#%f";
const char STRING_TO_FLOAT_FORMAT[32] = "%f#%f#%f#%f";


void replaceChacacter( char* str, char oldChar, char newChar )
{
    while( *str ){
        if( *str == oldChar ){
            *str = newChar;
        }
        str++;
    }
}


/***
 * 1. Initialization and destruction
 ***/

SelectionTransformation::SelectionTransformation() :
    SceneCommand( SceneCommandType::SELECTION_TRANSFORMATION, 0 ),
    transformationType_( SelectionTransformationType::TRANSLATION )
{
    setTransformationMagnitude( 0.0f, 0.0f, 0.0f, 0.0f );
}


SelectionTransformation::SelectionTransformation( UserID userID ) :
    SceneCommand( SceneCommandType::SELECTION_TRANSFORMATION, userID ),
    transformationType_( SelectionTransformationType::TRANSLATION  )
{
    setTransformationMagnitude( 0.0f, 0.0f, 0.0f, 0.0f );
}


SelectionTransformation::SelectionTransformation( const SelectionTransformation& b ) :
    SceneCommand( b ),
    transformationType_( b.transformationType_ )
{
    setTransformationMagnitude( b.angle_,
                                b.transformationMagnitude_[0],
                                b.transformationMagnitude_[1],
                                b.transformationMagnitude_[2] );
}


/***
 * 2. Packing and unpacking
 ***/

char* SelectionTransformation::pack( char* buffer ) const
{
    // Pack the SceneCommand fields.
    buffer = SceneCommand::pack( buffer );

    // Pack the transformation type.
    packer::pack( static_cast< std::uint8_t >( transformationType_ ), buffer );

    // Pack the transformation magnitude.
    packer::pack( transformationMagnitudeStr_, buffer, TRANSFORMATION_MAGNITUDE_STR_SIZE );

    // Return the buffer updated pointer.
    return buffer;
}


const char* SelectionTransformation::unpack( const char* buffer )
{
    std::uint8_t transformationType;
    int sscanfReturnValue = 0;

    // Unpack the SceneCommand fields.
    buffer = SceneCommand::unpack( buffer );

    // Unpack the transformation type.
    packer::unpack( transformationType, buffer );
    transformationType_ = static_cast< SelectionTransformationType >( transformationType );

    // Unpack the transformation magnitude (string format).
    packer::unpack( transformationMagnitudeStr_, buffer, TRANSFORMATION_MAGNITUDE_STR_SIZE );

    /*
    char* str = transformationMagnitudeStr_;
    while( *str ){
        if( *str == ',' ){
            *str = '.';
        }
        str++;
    }
    */

    // The string representation of the float values may have a different
    // decimal separator between machines. Replace the dots (used for network
    // transfer) with the local decimal separator.
    replaceChacacter( transformationMagnitudeStr_,
                      '.',
                      ( localeconv() )->decimal_point[0] );

    // Translate the previous transformation magnitude from string format to
    // float one.
    sscanfReturnValue = sscanf( transformationMagnitudeStr_,
                                STRING_TO_FLOAT_FORMAT,
                                &angle_,
                                &( transformationMagnitude_[0] ),
                                &( transformationMagnitude_[1] ),
                                &( transformationMagnitude_[2] )
                                );

    if( ( sscanfReturnValue < 4 ) || ( sscanfReturnValue == EOF ) ){
        throw std::runtime_error( std::string( "ERROR when unpacking transformation magnitude by sscanf: return value(" ) +
                                  std::to_string( sscanfReturnValue ) +
                                  std::string( ")" ) );
    }

    // Return the buffer updated pointer.
    return buffer;
}


/***
 * 3. Getters
 ***/


std::uint16_t SelectionTransformation::getPacketSize() const
{
    return SceneCommand::getPacketSize() +
            sizeof( transformationType_ ) +
            TRANSFORMATION_MAGNITUDE_STR_SIZE;
}


SelectionTransformationType SelectionTransformation::getTransformationType() const
{
    return transformationType_;
}


const float* SelectionTransformation::getTransformationMagnitude() const
{
    return transformationMagnitude_;
}


float SelectionTransformation::getAngle() const
{
    return angle_;
}


/***
 * 4. Setters
 ***/


void SelectionTransformation::setTransformationType( SelectionTransformationType transformationType )
{
    transformationType_ = transformationType;
}


void SelectionTransformation::setTranslation( float tx, float ty, float tz )
{
    // We are doing a translation.
    transformationType_ = SelectionTransformationType::TRANSLATION;

    // Set the transformation magnitude.
    setTransformationMagnitude( 0.0f, tx, ty, tz );
}


void SelectionTransformation::setTranslation( const float* direction )
{
    // We are doing a translation.
    transformationType_ = SelectionTransformationType::TRANSLATION;

    // Set the transformation magnitude.
    setTransformationMagnitude( 0.0f,
                                direction[0],
                                direction[1],
                                direction[2] );
}


void SelectionTransformation::setRotation( float angle, float vx, float vy, float vz )
{
    // We are doing a rotation.
    transformationType_ = SelectionTransformationType::ROTATION;

    // Set the transformation magnitude.
    setTransformationMagnitude( angle, vx, vy, vz );
}


void SelectionTransformation::setRotation( float angle, const float* axis )
{
    // We are doing a rotation.
    transformationType_ = SelectionTransformationType::ROTATION;

    // Set the transformation magnitude.
    setTransformationMagnitude(
            angle,
            axis[0],
            axis[1],
            axis[2] );
}


void SelectionTransformation::setScale( float sx, float sy, float sz )
{
    // We are doing a scale.
    transformationType_ = SelectionTransformationType::SCALE;

    // Set the transformation magnitude.
    setTransformationMagnitude( 0.0f, sx, sy, sz );
}


void SelectionTransformation::setScale( const float* magnitude )
{
    // We are doing a scale.
    transformationType_ = SelectionTransformationType::SCALE;

    // Set the transformation magnitude.
    setTransformationMagnitude(
            0.0f,
            magnitude[0],
            magnitude[1],
            magnitude[2] );
}


void SelectionTransformation::setTransformationMagnitude( float angle, float x, float y, float z )
{
    // Copy the angle.
    angle_ = angle;

    // Copy the transformation magnitude.
    transformationMagnitude_[0] = x;
    transformationMagnitude_[1] = y;
    transformationMagnitude_[2] = z;

    // Create the string representation of the transformation angle and vector.
    snprintf( transformationMagnitudeStr_,
              TRANSFORMATION_MAGNITUDE_STR_SIZE,
              FLOAT_TO_STRING_FORMAT,
              angle,
              x,
              y,
              z );

    // The string representation of the float values may have a different
    // decimal separator between machines. Replace it with a dot for
    // network transfer.
    replaceChacacter( transformationMagnitudeStr_,
                      ( localeconv() )->decimal_point[0],
                      '.' );
}


} // namespace como
