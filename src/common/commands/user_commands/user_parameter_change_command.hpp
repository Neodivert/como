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

#ifndef USER_PARAMETER_CHANGE_COMMAND_HPP
#define USER_PARAMETER_CHANGE_COMMAND_HPP

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


class UserParameterChangeCommand : public UserCommand
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
        UserParameterChangeCommand( UserID userID = 0 );
        UserParameterChangeCommand( UserID userID, PivotPointMode pivotPointMode );
        UserParameterChangeCommand( const UserParameterChangeCommand& b );
        UserParameterChangeCommand( UserParameterChangeCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~UserParameterChangeCommand() = default;


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
        UserParameterChangeCommand& operator=( const UserParameterChangeCommand& ) = delete;
        UserParameterChangeCommand& operator=( UserParameterChangeCommand&& ) = delete;
};

typedef std::shared_ptr< const UserParameterChangeCommand > UserParameterChangeCommandConstPtr;

} // namespace como

#endif // USER_PARAMETER_CHANGE_COMMAND_HPP
