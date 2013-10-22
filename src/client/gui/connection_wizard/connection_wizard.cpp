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

#include "connection_wizard.hpp"
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFormLayout>
#include <QRegExpValidator>
#include "connect_server_page.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

ConnectionWizard::ConnectionWizard( std::shared_ptr< ServerInterface > serverInterface )
{
    setPage( PAGE_INTRO, createIntroPage() );
    setPage( PAGE_CONNECT_SERVER, new ConnectServerPage( serverInterface ) );

    setStartId( PAGE_INTRO );
}


QWizardPage* ConnectionWizard::createIntroPage() const
{
    QWizardPage* introPage = nullptr;
    QRadioButton* createServerRadioButton = nullptr;
    QRadioButton* connectToServerRadioButton = nullptr;
    QVBoxLayout* layout = nullptr;

    // Create the radio buttons and set one of them as checked.
    createServerRadioButton = new QRadioButton( tr( "&Create a server" ) );
    connectToServerRadioButton = new QRadioButton( tr( "Connect to a server" ) );
    connectToServerRadioButton->setChecked( true );

    // Create the page's layout.
    layout = new QVBoxLayout;
    layout->addWidget( createServerRadioButton );
    layout->addWidget( connectToServerRadioButton );

    // Create the page and set its title and layout.
    introPage = new QWizardPage;
    introPage->setTitle( tr( "Introduction" ) );
    introPage->setLayout( layout );

    return introPage;
}


}
