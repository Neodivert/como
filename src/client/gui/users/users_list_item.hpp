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

#ifndef USERS_LIST_ITEM_HPP
#define USERS_LIST_ITEM_HPP

#include <QListWidgetItem>
#include <common/ids/user_id.hpp>

namespace como {

class UsersListItem : public QListWidgetItem
{
    private:
        UserID userID_;

    public:
        /***
         * 1. Construction
         ***/
        UsersListItem() = delete;
        UsersListItem( UserID id, std::string name, QIcon icon );
        UsersListItem( const UsersListItem& ) = delete;
        UsersListItem( UsersListItem&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~UsersListItem() = default;


        /***
         * 3. Getters
         ***/
        UserID getUserID() const;


        /***
         * 4. Operators
         ***/
        UsersListItem& operator = ( const UsersListItem& ) = delete;
        UsersListItem& operator = ( UsersListItem&& ) = delete;
};

} // namespace como

#endif // USERS_LIST_ITEM_HPP
