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

#include "command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

Command::Command( CommandTarget commandTarget, UserID userID ) :
    commandTarget_( commandTarget ),
    userID_( userID )
{
    // Register the following packables as members of this CompositePackable.
    addPackable( &commandTarget_ );
    addPackable( &userID_ );
}


Command::Command( const Command& b ) :
    CompositePackable( b ),
    commandTarget_( b.commandTarget_ ),
    userID_( b.userID_ )
{
    // Register the following packables as members of this CompositePackable.
    addPackable( &commandTarget_ );
    addPackable( &userID_ );
}


/***
 * 3. Getters
 ***/

CommandTarget Command::getTarget() const
{
    return commandTarget_.getValue();
}

UserID Command::getUserID() const
{
    return userID_.getValue();
}


/***
 * 4. Buffer pre reading
 ***/

CommandTarget Command::getTarget( const void* buffer )
{
    CompositePackable compositePackable;

    return static_cast< const CommandTarget* >( buffer )[compositePackable.getPacketSize()];
}


/***
 * 5. Setters
 ***/

void Command::setUserID( const UserID& userID )
{
    userID_ = userID;
}

} // namespace como
