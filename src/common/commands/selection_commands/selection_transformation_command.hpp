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
    ROTATION_AROUND_PIVOT,
    ROTATION_AROUND_INDIVIDUAL_CENTROIDS,
    SCALE_AROUND_PIVOT,
    SCALE_AROUND_INDIVIDUAL_CENTROIDS
};
typedef PackableUint8< SelectionTransformationCommandType > PackableSelectionTransformationCommandType;


class SelectionTransformationCommand : public SelectionCommand
{
    private:
        PackableSelectionTransformationCommandType transformationType_;
        PackableFloat transformationAngle_;
        PackableArray3< PackableFloat, float > transformationVector_;
        PackableArray3< PackableFloat, float > pivotPoint_;

    public:
        /***
         * 1. Construction
         ***/
        SelectionTransformationCommand();
        SelectionTransformationCommand( UserID userID );
        SelectionTransformationCommand( const SelectionTransformationCommand& b );
        SelectionTransformationCommand( SelectionTransformationCommand&& ) = delete;
        COMMAND_CLONE_METHOD( SelectionTransformationCommand )


        /***
         * 2. Destruction
         ***/
        virtual ~SelectionTransformationCommand() = default;


        /***
         * 3. Getters
         ***/
        SelectionTransformationCommandType getTransformationType() const ;
        glm::vec3 getTransformationVector() const ;
        std::unique_ptr< glm::vec3 > getPivotPoint() const;

        float getTransformationAngle() const ;


        /***
         * 4. Setters
         ***/
        void setTransformationType( SelectionTransformationCommandType transformationType );

        void setTranslation( const glm::vec3& direction );

        void setRotationAroundPivot( float angle, const glm::vec3& axis, const glm::vec3& pivot );
        void setRotationAroundIndividualCentroids( float angle, const glm::vec3& axis );

        void setScaleAroundPivot( const glm::vec3& scaleFactors, const glm::vec3& pivot );
        void setScaleAroundIndividualCentroids( const glm::vec3& scaleFactors );


    public:
        /***
         * 5. Operators
         ***/
        SelectionTransformationCommand& operator=( const SelectionTransformationCommand& ) = delete;
        SelectionTransformationCommand& operator=( SelectionTransformationCommand&& ) = delete;
};

} // namespace como

#endif // SELECTION_TRANSFORMATION_COMMAND_HPP
