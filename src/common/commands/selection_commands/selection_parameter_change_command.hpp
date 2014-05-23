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

#ifndef SELECTION_PARAMETER_CHANGE_COMMAND_HPP
#define SELECTION_PARAMETER_CHANGE_COMMAND_HPP

#include "abstract_selection_parameter_change_command.hpp"

namespace como {

template < SelectionParameterName ParameterName, class ParameterPackableType, class ParameterPlainType >
class SelectionParameterChangeCommand : public AbstractSelectionParameterChangeCommand
{
    private:
        ParameterPackableType parameterValue_;

    public:
        /***
         * 1. Construction
         ***/
        SelectionParameterChangeCommand();
        SelectionParameterChangeCommand( UserID userID, ParameterPackableType parameterValue );
        SelectionParameterChangeCommand( UserID userID, ParameterPlainType parameterValue );
        SelectionParameterChangeCommand( const SelectionParameterChangeCommand& b );
        SelectionParameterChangeCommand( SelectionParameterChangeCommand& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~SelectionParameterChangeCommand() = default;


        /***
         * 3. Getters
         ***/
        ParameterPlainType getParameterValue() const;


        /***
         * 4. Operators
         ***/
        SelectionParameterChangeCommand& operator = ( const SelectionParameterChangeCommand& ) = delete;
        SelectionParameterChangeCommand& operator = ( SelectionParameterChangeCommand&& ) = delete;
};

typedef SelectionParameterChangeCommand< SelectionParameterName::MATERIAL_COLOR, PackableColor, std::array< std::uint8_t, 4 > > SelectionMaterialColorChangeCommand;
typedef SelectionParameterChangeCommand< SelectionParameterName::MATERIAL_COLOR, PackableColor, std::array< std::uint8_t, 4 > > SelectionMaterialDiffuseReflectivityChangeCommand;
typedef SelectionParameterChangeCommand< SelectionParameterName::MATERIAL_COLOR, PackableColor, std::array< std::uint8_t, 4 > > SelectionMaterialSpecularReflectivityChangeCommand;
typedef SelectionParameterChangeCommand< SelectionParameterName::MATERIAL_COLOR, PackableFloat, float > SelectionMaterialSpecularExponentChangeCommand;


/***
 * 1. Construction
 ***/

template < SelectionParameterName ParameterName, class ParameterPackableType, class ParameterPlainType >
SelectionParameterChangeCommand<ParameterName, ParameterPackableType, ParameterPlainType>::SelectionParameterChangeCommand() :
    AbstractSelectionParameterChangeCommand( ParameterName, NO_USER )
{
    addPackable( &parameterValue_ );
}


template < SelectionParameterName ParameterName, class ParameterPackableType, class ParameterPlainType >
SelectionParameterChangeCommand<ParameterName, ParameterPackableType, ParameterPlainType>::SelectionParameterChangeCommand( UserID userID, ParameterPackableType parameterValue ) :
    AbstractSelectionParameterChangeCommand( ParameterName, userID ),
    parameterValue_( parameterValue )
{
    addPackable( &parameterValue_ );
}


template < SelectionParameterName ParameterName, class ParameterPackableType, class ParameterPlainType >
SelectionParameterChangeCommand<ParameterName, ParameterPackableType, ParameterPlainType>::SelectionParameterChangeCommand( UserID userID, ParameterPlainType parameterValue ) :
    AbstractSelectionParameterChangeCommand( ParameterName, userID ),
    parameterValue_( parameterValue )
{
    addPackable( &parameterValue_ );
}


template < SelectionParameterName ParameterName, class ParameterPackableType, class ParameterPlainType >
SelectionParameterChangeCommand<ParameterName, ParameterPackableType, ParameterPlainType>::SelectionParameterChangeCommand( const SelectionParameterChangeCommand& b ) :
    AbstractSelectionParameterChangeCommand( b ),
    parameterValue_( b.parameterValue_ )
{
    addPackable( &parameterValue_ );
}


/***
 * 3. Getters
 ***/

template < SelectionParameterName ParameterName, class ParameterPackableType, class ParameterPlainType >
ParameterPlainType SelectionParameterChangeCommand<ParameterName, ParameterPackableType, ParameterPlainType>::getParameterValue() const
{
    return parameterValue_.getValue();
}

} // namespace como

#endif // SELECTION_PARAMETER_CHANGE_COMMAND_HPP
