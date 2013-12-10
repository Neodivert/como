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

namespace como {

/***
 * 1. Initialization and destruction
 ***/

SelectionTransformation::SelectionTransformation() :
    SceneCommand( SceneCommandType::SELECTION_TRANSFORMATION, 0 ),
    transformationType_( SelectionTransformationType::TRANSLATION )
{
    setTransformationMagnitude( 0.0f, 0.0f, 0.0f );
}


SelectionTransformation::SelectionTransformation( UserID userID,
                                                  SelectionTransformationType transformationType ) :
    SceneCommand( SceneCommandType::SELECTION_TRANSFORMATION, userID ),
    transformationType_( transformationType )
{
    setTransformationMagnitude( 0.0f, 0.0f, 0.0f );
}


SelectionTransformation::SelectionTransformation( UserID userID,
                                                  SelectionTransformationType transformationType,
                                                  const float* transformationMagnitude ) :
    SceneCommand( SceneCommandType::SELECTION_TRANSFORMATION, userID ),
    transformationType_( transformationType )
{
    setTransformationMagnitude( transformationMagnitude );
}


SelectionTransformation::SelectionTransformation( const SelectionTransformation& b ) :
    SceneCommand( b ),
    transformationType_( b.transformationType_ )
{
    setTransformationMagnitude( b.transformationMagnitude_ );
}


/***
 * 2. Packing and unpacking
 ***/

char* SelectionTransformation::pack( char* buffer ) const
{
    unsigned int i;

    // Pack the SceneCommand fields.
    buffer = SceneCommand::pack( buffer );

    // Pack the transformation type.
    packer::pack( static_cast< std::uint8_t >( transformationType_ ), buffer );

    // Pack the transformation magnitude.
    for( i=0; i<3; i++ ){
        packer::pack( std::int32_t( transformationMagnitude_[i] * FLOAT_TO_INT_FACTOR ), buffer );
    }

    // Return the buffer updated pointer.
    return buffer;
}


const char* SelectionTransformation::unpack( const char* buffer )
{
    unsigned int i;
    std::uint8_t transformationType;
    std::int32_t transformationMagnitude;

    // Unpack the SceneCommand fields.
    buffer = SceneCommand::unpack( buffer );

    // Unpack the transformation type.
    packer::unpack( transformationType, buffer );
    transformationType_ = static_cast< SelectionTransformationType >( transformationType );

    // Unpack the transformation magnitude.
    for( i=0; i<3; i++ ){
        packer::unpack( transformationMagnitude, buffer );
        transformationMagnitude_[i] = transformationMagnitude * INT_TO_FLOAT_FACTOR;
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
            sizeof( transformationMagnitude_[0] ) * 3;
}


SelectionTransformationType SelectionTransformation::getTransformationType() const
{
    return transformationType_;
}


const float* SelectionTransformation::getTransformationMagnitude() const
{
    return transformationMagnitude_;
}


/***
 * 4. Setters
 ***/


void SelectionTransformation::setTransformationType( SelectionTransformationType transformationType )
{
    transformationType_ = transformationType;
}


void SelectionTransformation::setTransformationMagnitude( const float* transformationMagnitude )
{
    unsigned int i;

    for( i=0; i<3; i++ ){
        transformationMagnitude_[i] = transformationMagnitude[i];
    }
}


void SelectionTransformation::setTransformationMagnitude( const float& xTransf, const float& yTransf, const float& zTransf )
{
    transformationMagnitude_[0] = xTransf;
    transformationMagnitude_[1] = yTransf;
    transformationMagnitude_[2] = zTransf;
}


} // namespace como
