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

#include "primitive_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

PrimitiveCommand::PrimitiveCommand( PrimitiveCommandType primitiveCommandType, UserID userID, PrimitiveID primitiveID ) :
    Command( CommandTarget::PRIMITIVE, userID ),
    commandType_( primitiveCommandType ),
    primitiveID_( primitiveID )
{
    // Register the desired packable attributes as part of this
    // CompositePackable.
    addPackable( &commandType_ );
    addPackable( &primitiveID_ );
}


PrimitiveCommand::PrimitiveCommand( const PrimitiveCommand& b ) :
    Command( b ),
    commandType_( b.commandType_ ),
    primitiveID_( b.primitiveID_ )
{
    // Register the desired packable attributes as part of this
    // CompositePackable.
    addPackable( &commandType_ );
    addPackable( &primitiveID_ );
}


/***
 * 3. Getters
 ***/

PrimitiveCommandType PrimitiveCommand::getType() const
{
    return commandType_.getValue();
}


PrimitiveID PrimitiveCommand::getPrimitiveID() const
{
    return primitiveID_.getValue();
}


/***
 * 5. Buffer pre reading
 ***/

PrimitiveCommandType PrimitiveCommand::getType( const void* buffer )
{
    return *( static_cast< const PrimitiveCommandType* >( buffer ) );
}

} // namespace como
