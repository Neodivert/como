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

#include "user_connection_command.hpp"
#include <cstring>

namespace como {

/***
 * 1. Construction
 ***/

UserConnectionCommand::UserConnectionCommand( UserID userID ) :
    UserCommand( UserCommandType::USER_CONNECTION, userID ),
    name_( "Unnamed" ),
    selectionColor_()

{
    addPackable( &name_ );
    addPackable( &selectionColor_ );
}


UserConnectionCommand::UserConnectionCommand( const UserAcceptancePacket& userAcceptedPacket ) :
    UserCommand( UserCommandType::USER_CONNECTION, userAcceptedPacket.getId() ),
    name_( userAcceptedPacket.getName() ),
    selectionColor_( userAcceptedPacket.getSelectionColor() )
{
    addPackable( &name_ );
    addPackable( &selectionColor_ );
}


UserConnectionCommand::UserConnectionCommand( const UserConnectionCommand& b ) :
    UserCommand( b ),
    name_( b.name_ ),
    selectionColor_( b.selectionColor_ )
{
    addPackable( &name_ );
    addPackable( &selectionColor_ );
}


/***
 * 3. Getters
 ***/

const char* UserConnectionCommand::getName() const
{
    return name_.getValue();
}


const PackableColor& UserConnectionCommand::getSelectionColor() const
{
    return selectionColor_;
}


/***
 * 4. Setters
 ***/

void UserConnectionCommand::setName( const char* name )
{
    name_ = name;
}


void UserConnectionCommand::setSelectionColor( const std::uint8_t& r, const std::uint8_t& g, const std::uint8_t& b, const std::uint8_t& a )
{
    selectionColor_[0] = r;
    selectionColor_[1] = g;
    selectionColor_[2] = b;
    selectionColor_[3] = a;
}

} // namespace como
