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

#include "general_info_menu.hpp"

#include <QLabel>
#include <QFormLayout>

namespace como {

GeneralInfoMenu::GeneralInfoMenu( LocalDrawablesSelectionPtr userSelection ) :
    userSelection_( userSelection )
{
    // Create a layout for this widget.
    QFormLayout* layout = new QFormLayout;

    // Create a text input for manipulating the name of the currently selected
    // 3D object.
    objectName_ = new QLineEdit( "Unnamed" );
    objectName_->setReadOnly( true );

    // Create a label with the type of the object.
    objectType_ = new QLabel( "Unknown" );

    // Add widgets to the layout and set the latter as the current one.
    layout->addRow( new QLabel( "Name:" ), objectName_ );
    layout->addRow( new QLabel( "Type:" ), objectType_ );
    setLayout( layout );

    userSelection_->addObserver( this );
    update();
}


/***
 * 4. Refreshing
 ***/

void GeneralInfoMenu::update()
{
    objectName_->setText( userSelection_->getName().c_str() );
    objectType_->setText( userSelection_->getTypeName().c_str() );
}

} // namespace como
