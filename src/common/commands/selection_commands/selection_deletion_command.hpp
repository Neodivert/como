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

#ifndef SELECTION_DELETION_COMMAND_HPP
#define SELECTION_DELETION_COMMAND_HPP

#include "selection_command.hpp"

namespace como {

class SelectionDeletionCommand : public SelectionCommand
{
    public:
        /***
         * 1. Construction
         ***/
        SelectionDeletionCommand( UserID userID = 0 );
        SelectionDeletionCommand( const SelectionDeletionCommand& );
        SelectionDeletionCommand( SelectionDeletionCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~SelectionDeletionCommand() = default;


        /***
         * 3. Operators
         ***/
        SelectionDeletionCommand& operator=( const SelectionDeletionCommand& ) = delete;
        SelectionDeletionCommand& operator=( SelectionDeletionCommand&& ) = delete;
};

} // namespace como

#endif // SELECTION_DELETION_COMMAND_HPP
