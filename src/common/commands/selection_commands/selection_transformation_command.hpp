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

#ifndef SELECTION_TRANSFORMATION_COMMAND_HPP
#define SELECTION_TRANSFORMATION_COMMAND_HPP

#include "selection_command.hpp"

namespace como {

// Transformation types.
enum class SelectionTransformationCommandType : std::uint8_t
{
    TRANSLATION = 0,
    ROTATION,
    SCALE
};
typedef PackableUint8< SelectionTransformationCommandType > PackableSelectionTransformationCommandType;

const unsigned int TRANSFORMATION_MAGNITUDE_STR_SIZE = 32;


class SelectionTransformationCommand : public SelectionCommand
{
    private:
        PackableSelectionTransformationCommandType transformationType_;
        PackableVector4 transformationMagnitude_;

    public:
        /***
         * 1. Construction
         ***/
        SelectionTransformationCommand();
        SelectionTransformationCommand( UserID userID );
        SelectionTransformationCommand( const SelectionTransformationCommand& b );
        SelectionTransformationCommand( SelectionTransformationCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~SelectionTransformationCommand() = default;


        /***
         * 3. Getters
         ***/
        SelectionTransformationCommandType getTransformationType() const ;
        const float* getTransformationMagnitude() const ;
        float getAngle() const ;


        /***
         * 4. Setters
         ***/
        void setTransformationType( SelectionTransformationCommandType transformationType );

        void setTranslation( float tx, float ty, float tz );
        void setTranslation( const float* direction );

        void setRotation( float angle, float vx, float vy, float vz );
        void setRotation( float angle, const float* axis );

        void setScale( float sx, float sy, float sz );
        void setScale( const float* magnitude );

    public:
        /***
         * 5. Operators
         ***/
        SelectionTransformationCommand& operator=( const SelectionTransformationCommand& ) = delete;
        SelectionTransformationCommand& operator=( SelectionTransformationCommand&& ) = delete;
};

} // namespace como

#endif // SELECTION_TRANSFORMATION_COMMAND_HPP
