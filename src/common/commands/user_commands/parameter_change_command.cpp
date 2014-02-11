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

#include "parameter_change_command.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

ParameterChangeCommand::ParameterChangeCommand( UserID userID ) :
    UserCommand( UserCommandType::PARAMETER_CHANGE, userID ),
    parameterType_( ParameterType::PIVOT_POINT_MODE )
{
    value_.pivotPointMode_ = PivotPointMode::MEDIAN_POINT;
}


ParameterChangeCommand::ParameterChangeCommand( UserID userID, PivotPointMode pivotPointMode ) :
    UserCommand( UserCommandType::PARAMETER_CHANGE, userID ),
    parameterType_( ParameterType::PIVOT_POINT_MODE )
{
    value_.pivotPointMode_ = pivotPointMode;
}


ParameterChangeCommand::ParameterChangeCommand( const ParameterChangeCommand& b ) :
    UserCommand( b ),
    parameterType_( b.parameterType_ )
{
    value_ = b.value_;
}


/***
 * 3. Packing and unpacking
 ***/

char* ParameterChangeCommand::pack( char* buffer ) const
{
    // Pack UserCommand attributes.
    buffer = UserCommand::pack( buffer );

    // Pack ParameterCnageCommand attributes.
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


const char* ParameterChangeCommand::unpack( const char* buffer )
{
    std::uint8_t castedValue;

    // Unpack UserCommand fields.
    buffer = UserCommand::unpack( buffer );

    // Unpack ParameterCnageCommand attributes.
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
 * 4. Getters
 ***/

std::uint16_t ParameterChangeCommand::getPacketSize() const
{
    uint16_t packetSize = UserCommand::getPacketSize() +
            sizeof( parameterType_ );

    switch( parameterType_ ){
        case ParameterType::PIVOT_POINT_MODE:
            packetSize += sizeof( value_.pivotPointMode_ );
        break;
    }

    return packetSize;
}


ParameterType ParameterChangeCommand::getParameterType() const
{
    return parameterType_;
}


PivotPointMode ParameterChangeCommand::getPivotPointMode() const
{
    return value_.pivotPointMode_;
}


/***
 * 5. Setters
 ***/

void ParameterChangeCommand::setPivotPointMode( PivotPointMode pivotPointMode )
{
    // Set parameter type.
    parameterType_ = ParameterType::PIVOT_POINT_MODE;

    // Set parameter value.
    value_.pivotPointMode_ = pivotPointMode;
}


} // namespace como
