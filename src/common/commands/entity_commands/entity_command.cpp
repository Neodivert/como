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

#include "entity_command.hpp"

namespace como {


/***
 * 1. Construction
 ***/

EntityCommand::EntityCommand(EntityCommandType commandType, const ResourceID &entityID, UserID userID) :
    TypeCommand( CommandTarget::ENTITY, commandType, userID ),
    entityID_( entityID )
{
    addPackable( &entityID_ );
}


EntityCommand::EntityCommand( const EntityCommand &b ) :
    TypeCommand( b ),
    entityID_( b.entityID_ )
{
    addPackable( &entityID_ );
}


/***
 * 3. Getters
 ***/

ResourceID EntityCommand::entityID() const
{
    return entityID_.getValue();
}

} // namespace como
