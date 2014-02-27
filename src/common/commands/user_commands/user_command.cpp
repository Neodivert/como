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

#include "user_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

UserCommand::UserCommand( UserCommandType userCommandType, UserID userID ) :
    Command( CommandTarget::USER, userID ),
    commandType_( userCommandType )
{
    addBodyPackable( &commandType_ );
}


UserCommand::UserCommand( const UserCommand& b ) :
    Command( b ),
    commandType_( b.commandType_ )
{
    addBodyPackable( &commandType_ );
}


/***
 * 3. Getters
 ***/

UserCommandType UserCommand::getType() const
{
    return commandType_.getValue();
}


/***
 * 4. Buffer pre reading
 ***/

UserCommandType UserCommand::getType( const void* buffer )
{
    return *( static_cast< const UserCommandType* >( buffer ) );
}


} // namespace como
