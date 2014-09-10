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

#ifndef USERS_LIST_HPP
#define USERS_LIST_HPP

#include <QListWidget>
#include <common/commands/commands.hpp>
#include <common/utilities/log.hpp>
#include <map>
#include "users_list_item.hpp"
#include <common/utilities/observable_container/container_observer.hpp>
#include <client/managers/managers/users/users_manager.hpp>

namespace como {

class UsersList : public QListWidget, public ContainerObserver<UserID>
{
    Q_OBJECT

    private:
        // Log
        LogPtr log_;
        UsersManagerPtr usersManager_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        UsersList() = delete;
        UsersList( const UsersList& ) = delete;
        UsersList( UsersList&& ) = delete;
        UsersList( QWidget* parent, LogPtr log, UsersManagerPtr usersManager );

        ~UsersList() = default;


        /***
         * 2. Operators
         ***/
        UsersList& operator = ( const UsersList& ) = delete;
        UsersList& operator = ( UsersList&& ) = delete;


        /***
         * 3. Uptading (observer pattern)
         ***/
        virtual void update( ContainerAction lastContainerAction, UserID lastElementModified );


    protected:
        /***
         * 4. Users management
         ***/
        void addUser( const ColouredUser& user );
        void removeUser( UserID userID );
};

} // namespace como

#endif // USERS_LIST_HPP
