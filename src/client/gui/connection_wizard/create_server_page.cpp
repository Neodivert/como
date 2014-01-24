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

#include "create_server_page.hpp"
#include <QFormLayout>
#include <QIntValidator>

namespace como {


/***
 * 1. Initialization and destruction
 ***/

CreateServerPage::CreateServerPage( ScenePtr scene, LogPtr log ) :
    scene_( scene ),
    portInput_( nullptr ),
    maxUsersInput_( nullptr ),
    userNameInput_( nullptr ),
    log_( log )
{
    QFormLayout* layout = nullptr;

    // Create a port input.
    portInput_ = new QLineEdit;
    portInput_->setText( tr( "7777") );
    portInput_->setValidator( new QIntValidator( 0, 65535, portInput_ ) );

    // Create a max users input.
    maxUsersInput_ = new QSpinBox;
    maxUsersInput_->setMinimum( 1 );
    maxUsersInput_->setMaximum( 16 );
    maxUsersInput_->setValue( 5 );

    // Create a user name input.
    userNameInput_ = new QLineEdit;

    // Create the page's layout
    layout = new QFormLayout;
    layout->addRow( tr( "Port (0 - 65535): " ), portInput_ );
    layout->addRow( tr( "Max users (1 - 16): " ), maxUsersInput_ );
    layout->addRow( tr( "Your user name in the server" ), userNameInput_ );

    // Set the page's title and layout.
    setTitle( tr( "Create server" ) );
    setLayout( layout );
}


/***
 * 2. Validators
 ***/

bool CreateServerPage::validatePage()
{
    char serverCommand[128];
    int pid;

    // We consider this page valid if we can create the server and connect to
    // it with the connection info given by user.
    try{
        std::string userName = userNameInput_->text().toLocal8Bit().data();
        if( !userName.size() ){
            log_->error( "ERROR: User name can't be empty\n" );
            return false;
        }
        if( ( userName.find( '(' ) != std::string::npos ) ||
            ( userName.find( ')' ) != std::string::npos ) ){
            log_->error( "ERROR: User name can't contain parenthesis\n" );
            return false;
        }

        log_->debug( "Creating server with port(", portInput_->text().toLocal8Bit().data(), ") and maxUsers(", maxUsersInput_->text().toLocal8Bit().data(), ")\n" );

        pid = fork();
        if( pid == 0 ){
            // FIXME: This isn't multiplatform.
            sprintf( serverCommand, "gnome-terminal -e \"%s %d %d\"",
                                                SERVER_PATH,                                        // Server bin.
                                                atoi( portInput_->text().toLocal8Bit().data() ),    // Port.
                                                maxUsersInput_->value()                             // Max. users.
                     );
            log_->debug( "Server command: [", serverCommand, "]\n",
                         "\tReturn value: ", system( serverCommand ), "\n" );
            exit( 0 );
        }

        // FIXME: Maybe there is something more elegant?
        system( "sleep 1" );

        // Try to connect to the Server scene. In case of error, the method
        // Scene::connect() throws an error.
        scene_->connect( "127.0.0.1",                                 // Server IP
                         portInput_->text().toLocal8Bit().data(),     // Server port
                         userName.c_str()                             // User name
                       );
        return true;
    }catch( std::runtime_error& ex ){
        log_->error( ex.what(), "\n" );
        return false;
    }
}


/***
 * 3. Auxiliar methods
 ***/

int CreateServerPage::nextId() const
{
    return -1;
}

} // namespace como
