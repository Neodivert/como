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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef USERS_LIST_HPP
#define USERS_LIST_HPP

#include "../../common/packets/scene_commands/scene_commands.hpp"
#include "../../common/utilities/log.hpp"
#include <QListWidget>
#include <map>

namespace como {

class UsersList : public QListWidget
{
    Q_OBJECT

    private:
        std::map< UserID, std::string > userIDToName_;

        // Log
        LogPtr log_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        UsersList( QWidget* parent, LogPtr log );


        /***
         * 2. Users insertion / deletion
         ***/
    public slots:
        void addUser( UserConnectedConstPtr userConnectedPacket );
        void removeUser( UserID userID );
};

} // namespace como

#endif // USERS_LIST_HPP