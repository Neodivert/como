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

#include "resource_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

ResourceCommand::ResourceCommand( ResourceCommandType commandType ) :
    TypeCommand( CommandTarget::RESOURCE, commandType, NO_USER )
{
    addPackable( &resourceID_ );
}


ResourceCommand::ResourceCommand( ResourceCommandType commandType, UserID userID, const ResourceID resourceID ) :
    TypeCommand( CommandTarget::RESOURCE, commandType, userID ),
    resourceID_( resourceID )
{
    addPackable( &resourceID_ );
}


ResourceCommand::ResourceCommand(const ResourceCommand &b) :
    TypeCommand( b )
{
    addPackable( &resourceID_ );
}



} // namespace como
