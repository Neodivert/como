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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "sync_data.hpp"

namespace como {

/***
 * 1. Construction
 ***/

SyncData::SyncData( const Command* creationCommand ) :
    creationCommand_( ( creationCommand != nullptr ) ? creationCommand->clone() : nullptr )
{

}


/***
 * 3. Getters
 ***/

CommandConstPtr SyncData::getCreationCommand() const
{
    if( creationCommand_ != nullptr ){
        return CommandConstPtr( creationCommand_->clone() );
    }else{
        return nullptr;
    }
}

} // namespace como
