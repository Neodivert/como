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

#include "main_window.hpp"
#include "render_panel.hpp"
#include "tools_menu/tools_menu.hpp"
#include "connection_wizard/connection_wizard.hpp"
#include "users_list.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

MainWindow::MainWindow( QWidget* parent, shared_ptr< ComoApp > comoApp ) :
    QMainWindow( parent )
{
    ToolsMenu* toolsMenu = nullptr;
    RenderPanel* renderPanel = nullptr;
    UsersList* usersList = nullptr;

    // Set window's title and dimensions.
    setWindowTitle( tr( "Cooperative Modeller" ) );
    setFixedSize( 800, 600 );

    // Set background color.
    setStyleSheet( "background-color: #F3E2A9;" );

    // Create a instance of Log.
    log_ = LogPtr( new Log );

    // Create a instance of ComoApp.
    this->comoApp = comoApp;

    // Set a QFrame as the central widget. This frame will hold all others widgets.
    setCentralWidget( new QFrame( this ) );

    // Create a tools' menu.
    toolsMenu = new ToolsMenu( this, comoApp );
    toolsMenu->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

    // Create a render panel (set of four OpenGL canvas).
    renderPanel = new RenderPanel( this, comoApp );
    renderPanel->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    // Create the user's list.
    usersList = new UsersList( this, log_ );

    // FIXME: Study why this is necessary.
    qRegisterMetaType< UserConnectionCommandConstPtr >();
    qRegisterMetaType< UserConnectionCommandConstPtr >( "UserConnectionCommandConstPtr" );
    qRegisterMetaType< UserID >( "UserID" );

    // Signal / slot: when an user connects to the scene, add it to the GUI
    // user's list.
    connect( comoApp->getScene().get(), &Scene::userConnected, usersList, &UsersList::addUser );

    // Signal / slot: when an user disconnects from the scene, remove it from
    // the GUI user's list.
    connect( comoApp->getScene().get(), &Scene::userDisconnected, usersList, &UsersList::removeUser );

    // Signal / slot: when an user connects to a scene, add the scene name to
    // the window title.
    connect( comoApp->getScene().get(), &Scene::connectedToScene, [this]( const QString& sceneName ){
        setWindowTitle( sceneName + " - Cooperative Modeller" );
    });

    // Set window layout.
    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget( toolsMenu );
    layout->addWidget( renderPanel );
    layout->addWidget( usersList );

    centralWidget()->setLayout( layout );
}

} // namespace como
