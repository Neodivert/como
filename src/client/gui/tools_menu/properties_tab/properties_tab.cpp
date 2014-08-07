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

#include "properties_tab.hpp"
#include <QVBoxLayout>

namespace como {


/***
 * 1. Construction
 ***/

PropertiesTab::PropertiesTab( LocalDrawablesSelectionPtr userSelection ) :
    userSelection_( userSelection )
{
    // Create the layout for this widget.
    QVBoxLayout* layout = new QVBoxLayout;

    // Construct the General Info menu.
    generalInfoMenu_ = new GeneralInfoMenu( userSelection_ );

    // Construct the Mesh Info menu.
    meshInfoMenu_ = new MeshInfoMenu( userSelection_ );

    // Add required widgets to the layout and set the latter as the current
    // layout.
    layout->addWidget( generalInfoMenu_ );
    layout->addWidget( meshInfoMenu_ );
    setLayout( layout );

    // Every time the user's selection changes, refres this properties tab.
    userSelection->addObserver( this );

    // Perform a first updating.
    update();
}


/***
 * 3. Updating (Observer pattern)
 ***/

void PropertiesTab::update()
{
    generalInfoMenu_->refresh();

    // If the user selection contains meshes, make the mesh panel
    // visible to user and refresh it.
    if( userSelection_->contains( DrawableType::MESH ) ){
        meshInfoMenu_->setVisible( true );
        meshInfoMenu_->refresh();
    }else{
        meshInfoMenu_->setVisible( false );
    }
}

} // namespace como
