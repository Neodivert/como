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

#include "user_parameter_change_command.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

UserParameterChangeCommand::UserParameterChangeCommand( UserID userID ) :
    UserCommand( UserCommandType::PARAMETER_CHANGE, userID ),
    parameterType_( ParameterType::PIVOT_POINT_MODE )
{
    pivotPointMode_ = PivotPointMode::SELECTION_CENTROID;

    addPackable( &parameterType_ );
    addPackable( &pivotPointMode_ );
}


UserParameterChangeCommand::UserParameterChangeCommand( UserID userID, PivotPointMode pivotPointMode ) :
    UserCommand( UserCommandType::PARAMETER_CHANGE, userID ),
    parameterType_( ParameterType::PIVOT_POINT_MODE )
{
    pivotPointMode_ = pivotPointMode;

    addPackable( &parameterType_ );
    addPackable( &pivotPointMode_ );
}


UserParameterChangeCommand::UserParameterChangeCommand( const UserParameterChangeCommand& b ) :
    UserCommand( b ),
    parameterType_( b.parameterType_ )
{
    pivotPointMode_ = b.pivotPointMode_;

    addPackable( &parameterType_ );
    addPackable( &pivotPointMode_ );
}


/***
 * 4. Getters
 ***/

ParameterType UserParameterChangeCommand::getParameterType() const
{
    return parameterType_.getValue();
}


PivotPointMode UserParameterChangeCommand::getPivotPointMode() const
{
    return pivotPointMode_.getValue();
}


/***
 * 5. Setters
 ***/

void UserParameterChangeCommand::setPivotPointMode( PivotPointMode pivotPointMode )
{
    // Set parameter type.
    parameterType_ = ParameterType::PIVOT_POINT_MODE;

    // Set parameter value.
    pivotPointMode_ = pivotPointMode;
}


} // namespace como
