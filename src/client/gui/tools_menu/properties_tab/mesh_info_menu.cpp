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

MeshInfoMenu::MeshInfoMenu( MeshesSelection* userSelection ) :
    userSelection_( userSelection )
{
    // Create the layout for this tab.
    QFormLayout* layout = new QFormLayout;

    // Create a label for displaying the centroid of the user's selection.
    centroidPosition_ = new QLabel( "Undefined" );

    // Add widgets to the layout and set it as the current one.
    layout->addRow( "Centroid position:", centroidPosition_ );
    layout->addWidget( createVertexNormalsDisplayGroupBox( userSelection ) );
    setLayout( layout );

    userSelection->addObserver( this );
    update();
}


/***
 * 3. Initialization
 ***/

QGroupBox* MeshInfoMenu::createVertexNormalsDisplayGroupBox( MeshesSelection* meshesSelection )
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

    meshesSelection->addObserver( this );

    displayVertexNormalsGroupBox = new QGroupBox( "Display vertex normals" );
    displayVertexNormalsGroupBox->setLayout( layout );
    return displayVertexNormalsGroupBox;
}


/***
 * 4. Refreshing
 ***/

void MeshInfoMenu::update()
{
    char centroidStr[50] = {0};
    glm::vec4 centroid = userSelection_->getCentroid();

    setVisible( userSelection_->size() );
    if( !( userSelection_->size() ) ){
        return;
    }

    // Convert the requested centroid into a string.
    sprintf( centroidStr, "(%.3f, %.3f, %.3f)", centroid.x, centroid.y, centroid.z );

    // Write the previous "centroid string" to its corresponding label.
    centroidPosition_->setText( centroidStr );

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
