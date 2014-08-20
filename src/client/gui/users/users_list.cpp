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

#include "users_list.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

UsersList::UsersList( QWidget* parent, LogPtr log, UsersManagerPtr usersManager ) :
    QListWidget( parent ),
    log_( log ),
    usersManager_( usersManager )
{
    usersManager_->addObserver( this );
}


/***
 * 3. Uptading (observer pattern)
 ***/

void UsersList::update( ContainerAction lastContainerAction, UserID lastElementModified )
{
    switch( lastContainerAction ){
        case ContainerAction::ELEMENT_INSERTION:
            addUser( usersManager_->user( lastElementModified ) );
        break;
        case ContainerAction::ELEMENT_DELETION:
            removeUser( lastElementModified );
        break;
        default:
            // TODO: Complete.
        break;
    }
}


/***
 * 4. Users management
 ***/

void UsersList::addUser( const ColouredUser& user )
{
    PackableColor userSelectionColor;
    QPixmap pixmap( 50, 50 );

    // Retrieve user's selection color.
    userSelectionColor = user.color();

    // Generate a "icon" colored in user's color.
    pixmap.fill( QColor(
                     userSelectionColor[0].getValue(),
                 userSelectionColor[1].getValue(),
                userSelectionColor[2].getValue(),
                userSelectionColor[3].getValue()
            ));
    QIcon userIcon ( pixmap );

    // Add the user's name to the list.
    UsersListItem* newUser = new UsersListItem( user.getID(), user.getName(), userIcon );
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
           dynamic_cast< UsersListItem* >( *currentUser )->getUserID() != userID )
    {
        currentUser++;
    }

    // If found, delete the user.
    if( currentUser != users.end() ){
        log_->debug( "\n\n\nRemoved user [", userID, "] from users list (GUI)\n\n\n" );

        userToBeDeleted = takeItem( row( *currentUser ) );
        delete userToBeDeleted;
    }else{
        log_->error( "GUI users list - removing user (", userID, ") ...USER NOT FOUND\n" );
    }
}

} // namespace como
