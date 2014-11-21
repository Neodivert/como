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

#include "user.hpp"

namespace como {

/***
 * 1. Construction
 ***/

User::User( UserID id, std::string name ) :
    id_( id ),
    name_( name )
{}


/***
 * 3. Getters
 ***/

UserID User::getID() const
{
    return id_;
}


std::string User::getName() const
{
    return name_;
}

} // namespace como
