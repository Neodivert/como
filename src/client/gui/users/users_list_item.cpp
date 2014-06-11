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

#include "users_list_item.hpp"

namespace como {

/***
 * 1. Construction
 ***/

UsersListItem::UsersListItem( UserID id , std::string name, QIcon icon ) :
    QListWidgetItem( icon, name.c_str() ),
    userID_( id )
{}


/***
 * 3. Getters
 ***/

UserID UsersListItem::getUserID() const
{
    return userID_;
}

} // namespace como
