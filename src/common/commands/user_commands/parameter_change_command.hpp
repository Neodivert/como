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

#ifndef PARAMETER_CHANGE_COMMAND_HPP
#define PARAMETER_CHANGE_COMMAND_HPP

#include "user_command.hpp"

namespace como {

// Available pivot point modes.
enum class PivotPointMode : std::uint8_t
{
    MEDIAN_POINT = 0,
    INDIVIDUAL_CENTROIDS,
    WORLD_ORIGIN
};
typedef PackableUint8< PivotPointMode > PackablePivotPointMode;

enum class ParameterType : std::uint8_t
{
    PIVOT_POINT_MODE
};
typedef PackableUint8< ParameterType > PackableParameterType;


class ParameterChangeCommand : public UserCommand
{
    private:
        PackableParameterType parameterType_;

        // TODO: Transform the full union into a packable.
        // Adapt the packing and unpacking methods for switching between parameter types.
        // Adapt the getPacketSize() method.
        PackablePivotPointMode pivotPointMode_;


    public:
        /***
         * 1. Construction
         ***/
        ParameterChangeCommand( UserID userID = 0 );
        ParameterChangeCommand( UserID userID, PivotPointMode pivotPointMode );
        ParameterChangeCommand( const ParameterChangeCommand& b );
        ParameterChangeCommand( ParameterChangeCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ParameterChangeCommand() = default;


        /***
         * 3. Getters
         ***/
        ParameterType getParameterType() const ;
        PivotPointMode getPivotPointMode() const ;


        /***
         * 4. Setters
         ***/
        void setPivotPointMode( PivotPointMode pivotPointMode );


        /***
         * 5. Operators
         ***/
        ParameterChangeCommand& operator=( const ParameterChangeCommand& ) = delete;
        ParameterChangeCommand& operator=( ParameterChangeCommand&& ) = delete;
};

} // namespace como

#endif // PARAMETER_CHANGE_COMMAND_HPP
