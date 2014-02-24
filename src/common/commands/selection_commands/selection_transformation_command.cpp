/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "selection_transformation_command.hpp"
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

SelectionTransformationCommand::SelectionTransformationCommand() :
    SelectionCommand( SelectionCommandType::SELECTION_TRANSFORMATION, 0 ),
    transformationType_( SelectionTransformationCommandType::TRANSLATION )
{
    setTransformationMagnitude( 0.0f, 0.0f, 0.0f, 0.0f );
}


SelectionTransformationCommand::SelectionTransformationCommand( UserID userID ) :
    SelectionCommand( SelectionCommandType::SELECTION_TRANSFORMATION, userID ),
    transformationType_( SelectionTransformationCommandType::TRANSLATION  )
{
    setTransformationMagnitude( 0.0f, 0.0f, 0.0f, 0.0f );
}


SelectionTransformationCommand::SelectionTransformationCommand( const SelectionTransformationCommand& b ) :
    SelectionCommand( b ),
    transformationType_( b.transformationType_ )
{
    setTransformationMagnitude( b.angle_,
                                b.transformationMagnitude_[0],
                                b.transformationMagnitude_[1],
                                b.transformationMagnitude_[2] );
}


/***
 * 3. Packing and unpacking
 ***/

char* SelectionTransformationCommand::pack( char* buffer ) const
{
    // Pack SelectionCommand attributes.
    buffer = SelectionCommand::pack( buffer );

    // Pack SelectionTransformationCommand attributes.
    packer::pack( static_cast< std::uint8_t >( transformationType_ ), buffer );
    packer::pack( transformationMagnitudeStr_, buffer, TRANSFORMATION_MAGNITUDE_STR_SIZE );

    // Return the buffer updated pointer.
    return buffer;
}


const char* SelectionTransformationCommand::unpack( const char* buffer )
{
    std::uint8_t transformationType;
    int sscanfReturnValue = 0;

    // Unpack SelectionCommand attributes.
    buffer = SelectionCommand::unpack( buffer );

    // Unpack SelectionTransformationCommand attributes.
    packer::unpack( transformationType, buffer );
    transformationType_ = static_cast< SelectionTransformationCommandType >( transformationType );
    packer::unpack( transformationMagnitudeStr_, buffer, TRANSFORMATION_MAGNITUDE_STR_SIZE );

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
 * 4. Getters
 ***/


std::uint16_t SelectionTransformationCommand::getPacketSize() const
{
    return SelectionCommand::getPacketSize() +
            sizeof( transformationType_ ) +
            TRANSFORMATION_MAGNITUDE_STR_SIZE;
}


SelectionTransformationCommandType SelectionTransformationCommand::getTransformationType() const
{
    return transformationType_;
}


const float* SelectionTransformationCommand::getTransformationMagnitude() const
{
    return transformationMagnitude_;
}


float SelectionTransformationCommand::getAngle() const
{
    return angle_;
}


/***
 * 5. Setters
 ***/


void SelectionTransformationCommand::setTransformationType( SelectionTransformationCommandType transformationType )
{
    transformationType_ = transformationType;
}


void SelectionTransformationCommand::setTranslation( float tx, float ty, float tz )
{
    // We are doing a translation.
    transformationType_ = SelectionTransformationCommandType::TRANSLATION;

    // Set the transformation magnitude.
    setTransformationMagnitude( 0.0f, tx, ty, tz );
}


void SelectionTransformationCommand::setTranslation( const float* direction )
{
    // We are doing a translation.
    transformationType_ = SelectionTransformationCommandType::TRANSLATION;

    // Set the transformation magnitude.
    setTransformationMagnitude( 0.0f,
                                direction[0],
                                direction[1],
                                direction[2] );
}


void SelectionTransformationCommand::setRotation( float angle, float vx, float vy, float vz )
{
    // We are doing a rotation.
    transformationType_ = SelectionTransformationCommandType::ROTATION;

    // Set the transformation magnitude.
    setTransformationMagnitude( angle, vx, vy, vz );
}


void SelectionTransformationCommand::setRotation( float angle, const float* axis )
{
    // We are doing a rotation.
    transformationType_ = SelectionTransformationCommandType::ROTATION;

    // Set the transformation magnitude.
    setTransformationMagnitude(
            angle,
            axis[0],
            axis[1],
            axis[2] );
}


void SelectionTransformationCommand::setScale( float sx, float sy, float sz )
{
    // We are doing a scale.
    transformationType_ = SelectionTransformationCommandType::SCALE;

    // Set the transformation magnitude.
    setTransformationMagnitude( 0.0f, sx, sy, sz );
}


void SelectionTransformationCommand::setScale( const float* magnitude )
{
    // We are doing a scale.
    transformationType_ = SelectionTransformationCommandType::SCALE;

    // Set the transformation magnitude.
    setTransformationMagnitude(
            0.0f,
            magnitude[0],
            magnitude[1],
            magnitude[2] );
}


void SelectionTransformationCommand::setTransformationMagnitude( float angle, float x, float y, float z )
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
