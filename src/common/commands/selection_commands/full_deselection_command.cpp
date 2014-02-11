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

#include "full_deselection_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

FullDeselectionCommand::FullDeselectionCommand( UserID userID ) :
    SelectionCommand( SelectionCommandType::FULL_DESELECTION, userID )
{
}


FullDeselectionCommand::FullDeselectionCommand( const FullDeselectionCommand& b ) :
    SelectionCommand( b )
{
}


} // namespace como
