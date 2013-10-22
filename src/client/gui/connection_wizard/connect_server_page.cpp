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

#include "connect_server_page.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

ConnectServerPage::ConnectServerPage( std::shared_ptr< ServerInterface > serverInterface ) :
    serverInterface_( serverInterface ),
    ipInput_( nullptr ),
    portInput_( nullptr )
{
    QFormLayout* layout = nullptr;

    // Create the IP input.
    ipInput_ = new QLineEdit;
    ipInput_->setInputMask( QString( "000.000.000.000; ") );
    ipInput_->setText( tr( "127.0.0.1" ) );

    // Create the port input.
    portInput_ = new QLineEdit;
    portInput_->setValidator( new QIntValidator( 0, 65535 ) );
    portInput_->setText( tr( "7777") );

    // Create the user name input.
    userNameInput_ = new QLineEdit;

    // Create the page's layout.
    layout = new QFormLayout;
    layout->addRow( tr( "IP: " ), ipInput_ );
    layout->addRow( tr( "Port (0 - 65535): " ), portInput_ );
    layout->addRow( tr( "User name (Parenthesis not allowed): " ), userNameInput_ );

    // Set the page's title and layout.
    setTitle( tr( "Connect to server" ) );
    setLayout( layout );
}


/***
 * 2. Validators
 ***/

bool ConnectServerPage::validatePage()
{
    // We consider this page valid if we can connect to the server with the
    // connection info given by user.
    try{
        std::string userName = userNameInput_->text().toLocal8Bit().data();
        if( !userName.size() ){
            std::cerr << "ERROR: User name can't be empty" << std::endl;
            return false;
        }
        if( ( userName.find( '(' ) != std::string::npos ) ||
            ( userName.find( ')' ) != std::string::npos ) ){
            std::cerr << "ERROR: User name can't contain parenthesis" << std::endl;
            return false;
        }

        std::cout << "Connecting to (" << ipInput_->text().toLocal8Bit().data() << ":" << portInput_->text().toLocal8Bit().data() << ")..." << std::endl;
        serverInterface_->connect( ipInput_->text().toLocal8Bit().data(),       // Server IP
                                   portInput_->text().toLocal8Bit().data(),     // Server port
                                   userName.c_str()                             // User name
                                 );
        return true;
    }catch( std::runtime_error& ex ){
        std::cerr << ex.what() << std::endl;
        return false;
    }
}

} // namespace como
