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
 * 3. Users insertion / deletion
 ***/

void UsersList::addUser( UserConnectionCommandConstPtr userConnectedPacket )
{
    const std::uint8_t* userSelectionColor = nullptr;
    QPixmap pixmap( 50, 50 );

    // Add the new User-ID-to-list-index translation to the map.
    userIDToName_[ userConnectedPacket->getUserID() ] = userConnectedPacket->getName();

    // Retrieve user's selection color.
    userSelectionColor = userConnectedPacket->getSelectionColor();

    // Generate a "icon" colored in user's color.
    pixmap.fill( QColor(
                     userSelectionColor[0],
                 userSelectionColor[1],
                userSelectionColor[2],
                userSelectionColor[3]
            ));
    QIcon userIcon ( pixmap );

    // Add the user's name to the list.
    QListWidgetItem* newUser = new QListWidgetItem( userIcon, tr( userConnectedPacket->getName() ) );
    addItem( newUser );
}


void UsersList::removeUser( UserID userID )
{
    QListWidgetItem* userToBeDeleted = nullptr;
    QList<QListWidgetItem *> users;
    QList<QListWidgetItem *>::iterator currentUser;

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
    }else{
        log_->error( "GUI users list - removing user [", ( userIDToName_[ userID ] ).c_str(), "] (id: ", userID, ") ...USER NOT FOUND\n" );
    }
}

} // namespace como
