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
#include <client/gui/users/users_list.hpp>
#include <client/gui/menu_bar.hpp>

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

    setMenuBar( new MenuBar( comoApp->getScene()->getDrawablesManager(), comoApp->getScene()->getMeshesManager() ) );

    // Create a instance of Log.
    log_ = comoApp->getLog();

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
    setWindowTitle( ( comoApp->getScene()->getName() + " - Cooperative Modeller" ).c_str() );

    // Set window layout.
    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget( toolsMenu, 10 );
    layout->addWidget( renderPanel, 80 );
    layout->addWidget( usersList, 10 );

    centralWidget()->setLayout( layout );
}


MainWindow::~MainWindow()
{
    log_->debug( "MainWindow destructor\n" );
}

} // namespace como
