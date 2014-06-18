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

#include "primitive_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

PrimitiveCommand::PrimitiveCommand( PrimitiveCommandType primitiveCommandType, UserID userID, ResourceID primitiveID ) :
    TypeCommand( CommandTarget::PRIMITIVE, primitiveCommandType, userID ),
    primitiveID_( primitiveID )
{
    // Register the desired packable attributes as part of this
    // CompositePackable.
    addPackable( &primitiveID_ );
}


PrimitiveCommand::PrimitiveCommand( const PrimitiveCommand& b ) :
    TypeCommand( b ),
    primitiveID_( b.primitiveID_ )
{
    // Register the desired packable attributes as part of this
    // CompositePackable.
    addPackable( &primitiveID_ );
}


/***
 * 3. Getters
 ***/

ResourceID PrimitiveCommand::getPrimitiveID() const
{
    return primitiveID_.getValue();
}

} // namespace como
