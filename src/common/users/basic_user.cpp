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

#include "basic_user.hpp"
#include <common/packets/packets.hpp>
#include <cstring>

namespace como {

/***
 * 1. Initialization and destruction
 ***/

BasicUser::BasicUser( UserID id, std::string name ) :
    id_( id ),
    name_( name )
{}


/***
 * 2. Getters
 ***/

UserID BasicUser::getID() const
{
    UserID id;

    //mutex_.lock();
    id = id_;
    //mutex_.unlock();

    return id;
}


std::string BasicUser::getName() const
{
    //mutex_.lock();
    //mutex_.unlock();

    return name_;
}

} // namespace como
