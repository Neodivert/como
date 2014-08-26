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

#include "mesh_info_menu.hpp"
#include <QRadioButton>

namespace como {

/***
 * 1. Construction
 ***/

MeshInfoMenu::MeshInfoMenu( LocalMeshesSelection* userSelection ) :
    userSelection_( userSelection )
{
    // Create the layout for this tab.
    QFormLayout* layout = new QFormLayout;

    // Add widgets to the layout and set it as the current one.
    layout->addWidget( createVertexNormalsDisplayGroupBox( userSelection ) );
    setLayout( layout );

    userSelection->Observable::addObserver( this );
    update();
}


/***
 * 3. Initialization
 ***/

QGroupBox* MeshInfoMenu::createVertexNormalsDisplayGroupBox( LocalMeshesSelection* meshesSelection )
{
    QGroupBox* displayVertexNormalsGroupBox = nullptr;
    QVBoxLayout* layout = nullptr;

    displayVertexNormalsAlways_ = new QRadioButton( "Always" );
    QObject::connect( displayVertexNormalsAlways_, &QCheckBox::toggled, [=]( bool toggled ){
        if( toggled ){
            meshesSelection->displayVertexNormals( true );
        }
    });

    displayVertexNormalsNever_ = new QRadioButton( "Never" );
    QObject::connect( displayVertexNormalsNever_, &QCheckBox::toggled, [=]( bool toggled ){
        if( toggled ){
            meshesSelection->displayVertexNormals( false );
        }
    });

    layout = new QVBoxLayout;
    layout->addWidget( displayVertexNormalsAlways_ );
    layout->addWidget( displayVertexNormalsNever_ );

    meshesSelection->Observable::addObserver( this );

    displayVertexNormalsGroupBox = new QGroupBox( "Display vertex normals" );
    displayVertexNormalsGroupBox->setLayout( layout );
    return displayVertexNormalsGroupBox;
}


/***
 * 4. Refreshing
 ***/

void MeshInfoMenu::update()
{
    setVisible( userSelection_->size() );
    if( !( userSelection_->size() ) ){
        return;
    }

    switch( userSelection_->displaysVertexNormals() ){
        case ElementsMeetingCondition::ALL:
            displayVertexNormalsAlways_->setChecked( true );
        break;
        case ElementsMeetingCondition::NONE:
            displayVertexNormalsNever_->setChecked( true );
        break;
        case ElementsMeetingCondition::SOME:
            // The "setAutoExclusive() trick" is necessary for being able to
            // uncheck both radio buttons.
            displayVertexNormalsAlways_->setAutoExclusive( false );
            displayVertexNormalsNever_->setAutoExclusive( false );

            displayVertexNormalsAlways_->setChecked( false );
            displayVertexNormalsNever_->setChecked( false );

            displayVertexNormalsAlways_->setAutoExclusive( true );
            displayVertexNormalsNever_->setAutoExclusive( true );
        break;
    }
}

} // namespace como
