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

#include "parameter_change_command.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

ParameterChangeCommand::ParameterChangeCommand( UserID userID ) :
    UserCommand( UserCommandType::PARAMETER_CHANGE, userID ),
    parameterType_( ParameterType::PIVOT_POINT_MODE )
{
    pivotPointMode_ = PivotPointMode::MEDIAN_POINT;

    addPackable( &parameterType_ );
    addPackable( &pivotPointMode_ );
}


ParameterChangeCommand::ParameterChangeCommand( UserID userID, PivotPointMode pivotPointMode ) :
    UserCommand( UserCommandType::PARAMETER_CHANGE, userID ),
    parameterType_( ParameterType::PIVOT_POINT_MODE )
{
    pivotPointMode_ = pivotPointMode;

    addPackable( &parameterType_ );
    addPackable( &pivotPointMode_ );
}


ParameterChangeCommand::ParameterChangeCommand( const ParameterChangeCommand& b ) :
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

ParameterType ParameterChangeCommand::getParameterType() const
{
    return parameterType_.getValue();
}


PivotPointMode ParameterChangeCommand::getPivotPointMode() const
{
    return pivotPointMode_.getValue();
}


/***
 * 5. Setters
 ***/

void ParameterChangeCommand::setPivotPointMode( PivotPointMode pivotPointMode )
{
    // Set parameter type.
    parameterType_ = ParameterType::PIVOT_POINT_MODE;

    // Set parameter value.
    pivotPointMode_ = pivotPointMode;
}


} // namespace como
