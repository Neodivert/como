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

#ifndef ABSTRACT_SELECTION_PARAMETER_CHANGE_COMMAND_HPP
#define ABSTRACT_SELECTION_PARAMETER_CHANGE_COMMAND_HPP

#include "selection_command.hpp"

namespace como {

enum class SelectionParameterName : std::uint8_t {
    MATERIAL_COLOR = 0
};
typedef PackableUint8< SelectionParameterName > PackableSelectionParameterName;


class AbstractSelectionParameterChangeCommand : public SelectionCommand
{
    private:
        const PackableSelectionParameterName parameterName_;

    public:
        /***
         * 1. Construction
         ***/
        AbstractSelectionParameterChangeCommand( SelectionParameterName parameterName );
        AbstractSelectionParameterChangeCommand( SelectionParameterName parameterName, UserID userID );
        AbstractSelectionParameterChangeCommand( const AbstractSelectionParameterChangeCommand& );
        AbstractSelectionParameterChangeCommand( AbstractSelectionParameterChangeCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~AbstractSelectionParameterChangeCommand() = default;


        /***
         * 3. Getters
         ***/
        virtual SelectionParameterName getParameterName() const;


        /***
         * 4. Buffer pre-reading
         ***/
        static SelectionParameterName getParameterName( const void* buffer );


        /***
         * 5. Operators
         ***/
        AbstractSelectionParameterChangeCommand& operator = ( const AbstractSelectionParameterChangeCommand& ) = default;
        AbstractSelectionParameterChangeCommand& operator = ( AbstractSelectionParameterChangeCommand&& ) = delete;
};

} // namespace como

#endif // ABSTRACT_SELECTION_PARAMETER_CHANGE_COMMAND_HPP
