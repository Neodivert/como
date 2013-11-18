/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
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

#include "users_list.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

UsersList::UsersList( QWidget* parent, LogPtr log ) :
    QListWidget( parent ),
    log_( log )
{
}


/***
 * 2. Users insertion / deletion
 ***/

void UsersList::addUser( UserConnectedConstPtr userConnectedPacket )
{
    // Add the new User-ID-to-list-index translation to the map.
    log_->debug( "GUI users list - adding user [userID: ",
                 userConnectedPacket->getUserID(), "] = [name: ", userConnectedPacket->getName(),
                 "] ...\n" );
    userIDToName_[ userConnectedPacket->getUserID() ] = userConnectedPacket->getName();

    // Add the user's name to the list.
    addItem( tr( userConnectedPacket->getName() ) );

    log_->debug( "GUI users list - adding user [userID: ",
                 userConnectedPacket->getUserID(), "] = [index: ", userConnectedPacket->getName(),
                 "] ...OK\n" );
}


void UsersList::removeUser( ID userID )
{
    QListWidgetItem* userToBeDeleted = nullptr;
    QList<QListWidgetItem *> users;
    QList<QListWidgetItem *>::iterator currentUser;

    log_->lock();
    log_->debug( "GUI users list - removing user [", ( userIDToName_[ userID ] ).c_str(), "] (id: ", userID, ") ...\n" );

    // Get a list with all the items in the users list.
    users = findItems( "*", Qt::MatchWildcard );

    // Iterate over the previous list and find the user to be deleted.
    currentUser = users.begin();
    while( ( currentUser != users.end() ) &&
           strcmp(
               (*currentUser)->text().toLocal8Bit().data(),
               ( userIDToName_[userID] ).c_str()
            )
    ){
        currentUser++;
    }

    // If found, delete the user.
    if( currentUser != users.end() ){
        userToBeDeleted = takeItem( row( *currentUser ) );
        delete userToBeDeleted;

        log_->debug( "GUI users list - removing user [", ( userIDToName_[ userID ] ).c_str(), "] (id: ", userID, ") ...OK\n" );
    }else{
        log_->error( "GUI users list - removing user [", ( userIDToName_[ userID ] ).c_str(), "] (id: ", userID, ") ...USER NOT FOUND\n" );
    }

    log_->unlock();
}

} // namespace como
