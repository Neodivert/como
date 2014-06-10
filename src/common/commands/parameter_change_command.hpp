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

#ifndef PARAMETER_CHANGE_COMMAND_HPP
#define PARAMETER_CHANGE_COMMAND_HPP

#include "type_command.hpp"
#include <common/packables/packable_uint8.hpp>

#define TEMPLATE_ARGUMENTS_LIST_DEC template < CommandTarget commandTarget, class TCommandType, TCommandType commandType, class TPlainParameterName, class TPackableID, class TPlainID, class TPackableParameterValue, class TPlainParameterValue >
#define TEMPLATE_ARGUMENTS_LIST_DEF commandTarget, TCommandType, commandType, TPackableID, TPlainID, TPlainParameterName, TPackableParameterValue, TPlainParameterValue

namespace como {

TEMPLATE_ARGUMENTS_LIST_DEC
class ParameterChangeCommand : public TypeCommand< TCommandType >
{
    private:
        const PackableUint8< TPlainParameterName > parameterName_;
        TPackableParameterValue parameterValue_; // TODO: Make this a AbstractPackableWrapper.

    public:
        /***
         * 1. Construction
         ***/
        ParameterChangeCommand() = default;
        ParameterChangeCommand( UserID userID, TPlainParameterName parameterName, TPlainParameterValue parameterValue );
        ParameterChangeCommand( const ParameterChangeCommand& );
        ParameterChangeCommand( ParameterChangeCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ParameterChangeCommand() = default;


        /***
         * 3. Getters
         ***/
        virtual TPlainParameterName getParameterName() const;
        virtual TPlainParameterValue getParameterValue() const;


        /***
         * 4. Buffer pre-reading
         ***/
        static TPlainParameterName getParameterName( const void* buffer );


        /***
         * 5. Operators
         ***/
        ParameterChangeCommand& operator = ( const ParameterChangeCommand& ) = default;
        ParameterChangeCommand& operator = ( ParameterChangeCommand&& ) = delete;
};


/***
 * 1. Construction
 ***/

TEMPLATE_ARGUMENTS_LIST_DEC
ParameterChangeCommand< TEMPLATE_ARGUMENTS_LIST_DEF >::ParameterChangeCommand( UserID userID, TPlainParameterName parameterName, TPlainParameterValue parameterValue ) :
    TypeCommand< TCommandType >( commandTarget, commandType, userID ),
    parameterName_( parameterName ),
    parameterValue_( parameterValue )
{
    this->addPackable( &parameterName_ );
    this->addPackable( &parameterValue_ );
}


/***
 * 3. Getters
 ***/

TEMPLATE_ARGUMENTS_LIST_DEC
TPlainParameterName ParameterChangeCommand< TEMPLATE_ARGUMENTS_LIST_DEF >::getParameterName() const
{
    return parameterName_.getValue();
}


TEMPLATE_ARGUMENTS_LIST_DEC
TPlainParameterValue ParameterChangeCommand< TEMPLATE_ARGUMENTS_LIST_DEF >::getParameterValue() const
{
    return parameterValue_.getValue();
}


/***
 * 4. Buffer pre-reading
 ***/

TEMPLATE_ARGUMENTS_LIST_DEC
TPlainParameterName ParameterChangeCommand< TEMPLATE_ARGUMENTS_LIST_DEF >::getParameterName( const void* buffer )
{
    TypeCommand< TCommandType > auxCommand;

    return static_cast< TPlainParameterName >( ( static_cast< const std::uint8_t* >( buffer ) )[auxCommand.getPacketSize()] );
}

} // namespace como

#endif // PARAMETER_CHANGE_COMMAND_HPP
