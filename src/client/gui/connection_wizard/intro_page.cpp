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

#include "intro_page.hpp"
#include "page_ids.hpp"

namespace como {


/***
 * 1. Initialization and destruction
 ***/

IntroPage::IntroPage() :
    createServerRadioButton_( nullptr ),
    connectToServerRadioButton_( nullptr )
{
    QVBoxLayout* layout = nullptr;

    // Create the radio buttons and set one of them as checked.
    createServerRadioButton_ = new QRadioButton( tr( "&Create a server" ) );
    connectToServerRadioButton_ = new QRadioButton( tr( "Connect to a server" ) );
    connectToServerRadioButton_->setChecked( true );

    // Create the page's layout.
    layout = new QVBoxLayout;
    layout->addWidget( createServerRadioButton_ );
    layout->addWidget( connectToServerRadioButton_ );

    // Set this page's title and layout.
    setTitle( tr( "Introduction" ) );
    setLayout( layout );
}


/***
 * 2. Auxiliar methods
 ***/

int IntroPage::nextId() const
{
    if( createServerRadioButton_->isChecked() ){
        return PAGE_CREATE_SERVER;
    }else{
        return PAGE_CONNECT_SERVER;
    }
}

} // namespace como
