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

#include "selection_transformation_command.hpp"
#include <cstdio>
#include <stdexcept>

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

namespace como {


/***
 * 1. Initialization and destruction
 ***/

SelectionTransformationCommand::SelectionTransformationCommand() :
    SelectionCommand( SelectionCommandType::SELECTION_TRANSFORMATION, 0 ),
    transformationType_( SelectionTransformationCommandType::TRANSLATION )
{
    transformationAngle_.setValue( 0.0f );
    transformationVector_.setValues( 0.0f, 0.0f, 0.0f );
    pivotPoint_.setValues( 0.0f, 0.0f, 0.0f );

    addPackable( &transformationType_ );
    addPackable( &transformationAngle_ );
    addPackable( &transformationVector_ );
    addPackable( &pivotPoint_ );
}


SelectionTransformationCommand::SelectionTransformationCommand( UserID userID ) :
    SelectionCommand( SelectionCommandType::SELECTION_TRANSFORMATION, userID ),
    transformationType_( SelectionTransformationCommandType::TRANSLATION  )
{
    transformationAngle_.setValue( 0.0f );
    transformationVector_.setValues( 0.0f, 0.0f, 0.0f );
    pivotPoint_.setValues( 0.0f, 0.0f, 0.0f );

    addPackable( &transformationType_ );
    addPackable( &transformationAngle_ );
    addPackable( &transformationVector_ );
    addPackable( &pivotPoint_ );
}


SelectionTransformationCommand::SelectionTransformationCommand( const SelectionTransformationCommand& b ) :
    SelectionCommand( b ),
    transformationType_( b.transformationType_ ),
    transformationAngle_( b.transformationAngle_ ),
    transformationVector_( b.transformationVector_ ),
    pivotPoint_( b.pivotPoint_ )
{
    addPackable( &transformationType_ );
    addPackable( &transformationAngle_ );
    addPackable( &transformationVector_ );
    addPackable( &pivotPoint_ );
}


/***
 * 3. Getters
 ***/

SelectionTransformationCommandType SelectionTransformationCommand::getTransformationType() const
{
    return transformationType_.getValue();
}


glm::vec3 SelectionTransformationCommand::getTransformationVector() const
{
    return glm::vec3( transformationVector_.getValues()[0],
            transformationVector_.getValues()[1],
            transformationVector_.getValues()[2] );
}


std::unique_ptr<glm::vec3> SelectionTransformationCommand::getPivotPoint() const
{
    if( ( transformationType_.getValue() == SelectionTransformationCommandType::ROTATION_AROUND_PIVOT ) ||
        ( transformationType_.getValue() == SelectionTransformationCommandType::SCALE_AROUND_PIVOT ) ){
        return std::unique_ptr< glm::vec3 >(
                    new glm::vec3(
                        pivotPoint_.getValues()[0],
                        pivotPoint_.getValues()[1],
                        pivotPoint_.getValues()[2] ) );
    }else{
        return nullptr;
    }
}


float SelectionTransformationCommand::getTransformationAngle() const
{
    return transformationAngle_.getValue();
}


/***
 * 4. Setters
 ***/


void SelectionTransformationCommand::setTransformationType( SelectionTransformationCommandType transformationType )
{
    transformationType_ = transformationType;
}


void SelectionTransformationCommand::setTranslation( const glm::vec3& direction )
{
    // We are doing a translation.
    transformationType_ = SelectionTransformationCommandType::TRANSLATION;

    // Set the transformation magnitude.
    transformationVector_.setValues( direction.x, direction.y, direction.z );
}


void SelectionTransformationCommand::setRotationAroundPivot( float angle, const glm::vec3 &axis, const glm::vec3 &pivot )
{
    transformationType_ = SelectionTransformationCommandType::ROTATION_AROUND_PIVOT;

    // Set the transformation magnitude.
    transformationAngle_.setValue( angle );
    transformationVector_.setValues( axis.x, axis.y, axis.z );
    pivotPoint_.setValues( pivot.x, pivot.y, pivot.z );
}


void SelectionTransformationCommand::setRotationAroundIndividualCentroids( float angle, const glm::vec3 &axis )
{
    transformationType_ = SelectionTransformationCommandType::ROTATION_AROUND_INDIVIDUAL_CENTROIDS;

    // Set the transformation magnitude.
    transformationAngle_.setValue( angle );
    transformationVector_.setValues( axis.x, axis.y, axis.z );
}


void SelectionTransformationCommand::setScaleAroundPivot(const glm::vec3 &scaleFactors, const glm::vec3 &pivot)
{
    transformationType_ = SelectionTransformationCommandType::SCALE_AROUND_PIVOT;

    // Set the transformation magnitude.
    transformationVector_.setValues( scaleFactors.x, scaleFactors.y, scaleFactors.z );
    pivotPoint_.setValues( pivot.x, pivot.y, pivot.z );
}


void SelectionTransformationCommand::setScaleAroundIndividualCentroids(const glm::vec3 &scaleFactors )
{
    transformationType_ = SelectionTransformationCommandType::SCALE_AROUND_INDIVIDUAL_CENTROIDS;

    // Set the transformation magnitude.
    transformationVector_.setValues( scaleFactors.x, scaleFactors.y, scaleFactors.z );
}

} // namespace como
