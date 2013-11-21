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

#include "main_window.hpp"
#include "render_panel.hpp"
#include "tools_menu.hpp"
#include "connection_wizard/connection_wizard.hpp"
#include "users_list.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent ),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet( "background-color: #F3E2A9;" );

    ToolsMenu* toolsMenu;
    RenderPanel* renderPanel;
    UsersList* usersList;

    // Create a instance of Log.
    log_ = LogPtr( new Log );

    // Create a instance of ComoApp.
    comoApp = shared_ptr< ComoApp >( new ComoApp( this, log_ ) );

    // Set a QFrame as the central widget. This frame will hold all others widgets.
    setCentralWidget( new QFrame );

    // Create a tools' menu.
    toolsMenu = new ToolsMenu( comoApp );
    toolsMenu->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

    // Create a render panel (set of four OpenGL canvas).
    renderPanel = new RenderPanel( comoApp );
    renderPanel->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    // Create the user's list.
    usersList = new UsersList( this, log_ );

    // FIXME: Study why this is necessary.
    qRegisterMetaType< UserConnectedConstPtr >();
    qRegisterMetaType< UserConnectedConstPtr >( "UserConnectedConstPtr" );
    qRegisterMetaType< UserID >( "UserID" );

    // Signal / slot: when an user connects to the scene, add it to the GUI
    // user's list.
    connect( comoApp->getScene().get(), &Scene::userConnected, usersList, &UsersList::addUser );

    // Signal / slot: when an user disconnects from the scene, remove it from
    // the GUI user's list.
    connect( comoApp->getScene().get(), &Scene::userDisconnected, usersList, &UsersList::removeUser );

    // Set window layout.
    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget( toolsMenu );
    layout->addWidget( renderPanel );
    layout->addWidget( usersList );

    centralWidget()->setLayout( layout );

    ConnectionWizard connectionWizard( comoApp->getScene(), log_, this );
    connectionWizard.exec();
}


MainWindow::~MainWindow()
{
    log_->debug( "Main window - destructor\n" );
    delete ui;
}


} // namespace como
