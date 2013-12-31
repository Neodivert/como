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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "change_parameter.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

ChangeParameter::ChangeParameter() :
    SceneCommand( SceneCommandType::CHANGE_PARAMETER ),
    parameterType_( ParameterType::PIVOT_POINT_MODE )
{
    value_.pivotPointMode_ = PivotPointMode::MEDIAN_POINT;
}


ChangeParameter::ChangeParameter( UserID userID ) :
    SceneCommand( SceneCommandType::CHANGE_PARAMETER, userID ),
    parameterType_( ParameterType::PIVOT_POINT_MODE )
{
    value_.pivotPointMode_ = PivotPointMode::MEDIAN_POINT;
}


ChangeParameter::ChangeParameter( UserID userID, PivotPointMode pivotPointMode ) :
    SceneCommand( SceneCommandType::CHANGE_PARAMETER, userID ),
    parameterType_( ParameterType::PIVOT_POINT_MODE )
{
    value_.pivotPointMode_ = pivotPointMode;
}


ChangeParameter::ChangeParameter( const ChangeParameter& b ) :
    SceneCommand( b ),
    parameterType_( b.parameterType_ )
{
    value_ = b.value_;
}


/***
 * 2. Packing and unpacking
 ***/

char* ChangeParameter::pack( char* buffer ) const
{
    // Pack SceneCommand fields.
    buffer = SceneCommand::pack( buffer );

    // Pack the parameter type.
    packer::pack( static_cast< std::uint8_t >( parameterType_ ), buffer );

    // Pack the parameter value (union).
    switch( parameterType_ ){
        case ParameterType::PIVOT_POINT_MODE:
            packer::pack( static_cast< std::uint8_t >( value_.pivotPointMode_ ), buffer );
        break;
    }

    // Return the buffer's updated pointer.
    return buffer;
}


const char* ChangeParameter::unpack( const char* buffer )
{
    std::uint8_t castedValue;

    // Unpack SceneCommand fields.
    buffer = SceneCommand::unpack( buffer );

    // Unpack the parameter type.
    packer::unpack( castedValue, buffer );
    parameterType_ = static_cast< ParameterType >( castedValue );

    // Unpack the parameter value (union).
    switch( parameterType_ ){
        case ParameterType::PIVOT_POINT_MODE:
            packer::unpack( castedValue, buffer );
            value_.pivotPointMode_ = static_cast< PivotPointMode >( castedValue );
        break;
    }

    // Return the buffer's updated pointer.
    return buffer;
}


/***
 * 3. Getters
 ***/

std::uint16_t ChangeParameter::getPacketSize() const
{
    uint16_t packetSize = SceneCommand::getPacketSize() +
            sizeof( parameterType_ );

    switch( parameterType_ ){
        case ParameterType::PIVOT_POINT_MODE:
            packetSize += sizeof( value_.pivotPointMode_ );
        break;
    }

    return packetSize;
}


ParameterType ChangeParameter::getParameterType() const
{
    return parameterType_;
}


PivotPointMode ChangeParameter::getPivotPointMode() const
{
    return value_.pivotPointMode_;
}


/***
 * 4. Setters
 ***/

void ChangeParameter::setPivotPointMode( PivotPointMode pivotPointMode )
{
    // Set parameter type.
    parameterType_ = ParameterType::PIVOT_POINT_MODE;

    // Set parameter value.
    value_.pivotPointMode_ = pivotPointMode;
}


} // namespace como
