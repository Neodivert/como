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
    transformationMagnitude_.setValue( 0.0f, 0.0f, 0.0f, 0.0f );

    addPackable( &transformationType_ );
    addPackable( &transformationMagnitude_ );
}


SelectionTransformationCommand::SelectionTransformationCommand( UserID userID ) :
    SelectionCommand( SelectionCommandType::SELECTION_TRANSFORMATION, userID ),
    transformationType_( SelectionTransformationCommandType::TRANSLATION  )
{
    transformationMagnitude_.setValue( 0.0f, 0.0f, 0.0f, 0.0f );

    addPackable( &transformationType_ );
    addPackable( &transformationMagnitude_ );
}


SelectionTransformationCommand::SelectionTransformationCommand( const SelectionTransformationCommand& b ) :
    SelectionCommand( b ),
    transformationType_( b.transformationType_ ),
    transformationMagnitude_( b.transformationMagnitude_ )
{
    addPackable( &transformationType_ );
    addPackable( &transformationMagnitude_ );
}


/***
 * 3. Getters
 ***/

SelectionTransformationCommandType SelectionTransformationCommand::getTransformationType() const
{
    return transformationType_.getValue();
}


const float* SelectionTransformationCommand::getTransformationMagnitude() const
{
    return transformationMagnitude_.getValue();
}


float SelectionTransformationCommand::getAngle() const
{
    return transformationMagnitude_[3];
}


/***
 * 4. Setters
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
    transformationMagnitude_.setValue( tx, ty, tz, 0.0f );
}


void SelectionTransformationCommand::setTranslation( const float* direction )
{
    // We are doing a translation.
    transformationType_ = SelectionTransformationCommandType::TRANSLATION;

    // Set the transformation magnitude.
    transformationMagnitude_.setValue( direction[0],
                                        direction[1],
                                        direction[2],
                                        direction[3] );
}


void SelectionTransformationCommand::setRotation( float angle, float vx, float vy, float vz )
{
    // We are doing a rotation.
    transformationType_ = SelectionTransformationCommandType::ROTATION;

    // Set the transformation magnitude.
    transformationMagnitude_.setValue( vx, vy, vz, angle );
}


void SelectionTransformationCommand::setRotation( float angle, const float* axis )
{
    // We are doing a rotation.
    transformationType_ = SelectionTransformationCommandType::ROTATION;

    // Set the transformation magnitude.
    transformationMagnitude_.setValue( axis[0], axis[1], axis[2], angle );
}


void SelectionTransformationCommand::setScale( float sx, float sy, float sz )
{
    // We are doing a scale.
    transformationType_ = SelectionTransformationCommandType::SCALE;

    // Set the transformation magnitude.
    transformationMagnitude_.setValue( sx, sy, sz, 0.0f );
}


void SelectionTransformationCommand::setScale( const float* magnitude )
{
    // We are doing a scale.
    transformationType_ = SelectionTransformationCommandType::SCALE;

    // Set the transformation magnitude.
    transformationMagnitude_.setValue( magnitude[0], magnitude[1], magnitude[2], magnitude[3] );
}

} // namespace como
