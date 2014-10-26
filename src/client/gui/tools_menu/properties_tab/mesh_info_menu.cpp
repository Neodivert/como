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

#include <QRadioButton>
#include <QFormLayout>
#include "mesh_info_menu.hpp"


namespace como {

/***
 * 1. Construction
 ***/

MeshInfoMenu::MeshInfoMenu( LocalMeshesSelection* userSelection ) :
    userSelection_( userSelection )
{
    displayVertexNormalsCheckBox_ = createVertexNormalsDisplayCheckBox( userSelection );

    // Set this widget's layout.
    QFormLayout* layout = new QFormLayout;
    layout->setRowWrapPolicy( QFormLayout::WrapAllRows );
    layout->addWidget( displayVertexNormalsCheckBox_ );
    setLayout( layout );

    userSelection->Observable::addObserver( this );
    setVisible( false );
}


/***
 * 3. Updating (observer pattern)
 ***/

void MeshInfoMenu::update()
{
    setVisible( userSelection_->size() );
    if( !( userSelection_->size() ) ){
        return;
    }

    displayVertexNormalsCheckBox_->blockSignals( true );
    displayVertexNormalsCheckBox_->setChecked(
                userSelection_->displaysVertexNormals() == ElementsMeetingCondition::ALL );
    displayVertexNormalsCheckBox_->blockSignals( false );
}


/***
 * 5. Initialization
 ***/

QCheckBox* MeshInfoMenu::createVertexNormalsDisplayCheckBox( LocalMeshesSelection* meshesSelection )
{
    QCheckBox* displayVertexNormalsCheckBox = new QCheckBox( "Display vertex normals" );

    QObject::connect( displayVertexNormalsCheckBox,
                      &QCheckBox::toggled,
                      [=]( bool toggled ){
        meshesSelection->displayVertexNormals( toggled );
    });

    return displayVertexNormalsCheckBox;
}

} // namespace como
