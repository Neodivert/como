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

#ifndef SELECTION_TRANSFORMATION_HPP
#define SELECTION_TRANSFORMATION_HPP

#include "scene_command.hpp"

namespace como {

// Transformation types.
enum class SelectionTransformationType
{
    TRANSLATION = 0,
    ROTATION,
    SCALE
};


// TODO: This constants reduce the effective range for transformation
// magnitudes. Make it clear somewhere.
const float FLOAT_TO_INT_FACTOR = 100.0f;
const float INT_TO_FLOAT_FACTOR = 1.0f / FLOAT_TO_INT_FACTOR;

const unsigned int TRANSFORMATION_MAGNITUDE_STR_SIZE = 32;


class SelectionTransformation : public SceneCommand
{
    private:
        // Transformation type.
        SelectionTransformationType transformationType_;

        // Angle (for rotations).
        float angle_;

        // Transformation magnitude (in every axis).
        float transformationMagnitude_[3];

        // Transformation magnitude (in string format for network
        // communication).
        char transformationMagnitudeStr_[TRANSFORMATION_MAGNITUDE_STR_SIZE];

    public:
        /***
         * 1. Initialization and destruction
         ***/
        SelectionTransformation();
        SelectionTransformation( UserID userID );
        SelectionTransformation( const SelectionTransformation& b );

        virtual ~SelectionTransformation(){}


        /***
         * 2. Packing and unpacking
         ***/
        virtual char* pack( char* buffer ) const ;
        virtual const char* unpack( const char* buffer ) ;


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const ;
        SelectionTransformationType getTransformationType() const ;
        const float* getTransformationMagnitude() const ;
        float getAngle() const ;


        /***
         * 4. Setters
         ***/
        void setTransformationType( SelectionTransformationType transformationType );

        void setTranslation( float tx, float ty, float tz );
        void setTranslation( const float* direction );

        void setRotation( float angle, float vx, float vy, float vz );
        void setRotation( float angle, const float* axis );

        void setScale( float sx, float sy, float sz );
        void setScale( const float* magnitude );

    private:
        void setTransformationMagnitude( float angle, float x, float y, float z );
};

} // namespace como

#endif // SELECTION_TRANSFORMATION_HPP
