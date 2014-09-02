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

#ifndef SELECTION_TRANSFORMATION_COMMAND_HPP
#define SELECTION_TRANSFORMATION_COMMAND_HPP

#include "selection_command.hpp"
#include <common/packables/packable_float.hpp>
#include <common/packables/array/packable_array_3.hpp>

namespace como {

// Transformation types.
enum class SelectionTransformationCommandType : std::uint8_t
{
    TRANSLATION = 0,
    ROTATION,
    SCALE
};
typedef PackableUint8< SelectionTransformationCommandType > PackableSelectionTransformationCommandType;


class SelectionTransformationCommand : public SelectionCommand
{
    private:
        PackableSelectionTransformationCommandType transformationType_;
        PackableFloat transformationAngle_;
        PackableArray3< PackableFloat, float > transformationVector_;


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
        virtual ~SelectionTransformationCommand() = default;


        /***
         * 3. Getters
         ***/
        SelectionTransformationCommandType getTransformationType() const ;
        const std::array< float, 3 > getTransformationVector() const ;
        float getTransformationAngle() const ;


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
