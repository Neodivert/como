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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "creation_tab.hpp"

namespace como {

/***
 * 1. Construction
 ***/

CreationTab::CreationTab( ScenePtr scene ) :
    scene_( scene ),
    meshColor_( 255, 0, 0, 255 )
{
    // Create a layout for this tab.
    QVBoxLayout* layout = new QVBoxLayout;

    // Add widgets to the previous layout.
    layout->addWidget( createMeshColorSelector() );
    layout->addWidget( createMeshFromPrimitiveCreationMenu() );

    // Set the previous layout as this tab's layout.
    setLayout( layout );
}


/***
 * 2. Initialization
 ***/

QFrame* CreationTab::createMeshColorSelector()
{
    QPushButton* selectColorButton = nullptr;
    QFrame* colorSelectorFrame = nullptr;
    QVBoxLayout* layout = nullptr;
    QLabel* currentColorLabel_ = nullptr;

    // Create the frame that whill hold the color's button and label.
    colorSelectorFrame = new QFrame( this );

    // Create a label showing the current selected color. The color name will
    // be colored in that color.
    currentColorLabel_ = new QLabel( "Current color: <font color=\"" + meshColor_.name() + "\">" + meshColor_.name() + "</font>" );

    // Signal / Slot connection. When the current color changes, change the
    // previous label's text accordingly.
    QObject::connect( this, &CreationTab::meshColorChanged, [=,this]( QColor newColor ){
        currentColorLabel_->setText( "Current color: <font color=\"" + newColor.name() + "\">" + newColor.name() + "</font>" );
    });

    // Create a button for changing the current color.
    selectColorButton = new QPushButton( tr( "Select color" ) );

    // Signal / Slot connection. When previous button is clicked, we invoke the
    // changeCurrentColor method.
    QObject::connect( selectColorButton, &QPushButton::clicked, [=,this](){
        // Open a dialog for selecting a new color.
        meshColor_ = QColorDialog::getColor( meshColor_ );

        // If the selected color is not valid, change it to red.
        if( !( meshColor_.isValid() ) ){
            meshColor_.setRgb( 255, 0, 0 );
        }

        // Emit a signal indicating that the current color has changed.
        emit meshColorChanged( meshColor_ );
    });

    // Set the frame's layout.
    layout = new QVBoxLayout;
    layout->addWidget( currentColorLabel_ );
    layout->addWidget( selectColorButton );
    colorSelectorFrame->setLayout( layout );

    // Return the frame.
    return colorSelectorFrame;
}

QFrame* CreationTab::createMeshFromPrimitiveCreationMenu()
{
    QFrame* frame = nullptr;
    QVBoxLayout* layout = nullptr;
    QComboBox* primitiveSelector = nullptr;
    QLabel* label = nullptr;

    // Create an empty dropdown list (QComboBox).
    primitiveSelector = new QComboBox;

    // Create a label for this menu.
    label = new QLabel( "Create mesh from primitive: " );

    // Create the layout of this menu.
    layout = new QVBoxLayout();
    layout->addWidget( label );
    layout->addWidget( primitiveSelector );

    // Create the container frame and set its layout.
    frame = new QFrame();
    frame->setLayout( layout );

    // Signal / Slot connection: when one of the available primitives is
    // selected, create a mesh from it and add it to the scene.
    void (QComboBox::*activated)( int ) = &QComboBox::activated;
    connect( primitiveSelector, activated, [=,this]( int primitiveID ) {
        uint8_t meshColor[4];
        int auxColor[4];
        meshColor_.getRgb( &auxColor[0], &auxColor[1], &auxColor[2], &auxColor[3] );

        for( unsigned int i=0; i<4; i++ ){
            meshColor[i] = auxColor[i];
        }

        scene_->getDrawablesManager()->addMesh( static_cast< PrimitiveID >( primitiveID ), meshColor );
    } );

    // Signal / Slot connection: when a new primitive is created in the scene,
    // add it to the primitives dropdown list.
    connect( scene_.get(), &Scene::primitiveAdded, [=]( const QString& primitiveRelPath, PrimitiveID primitiveID ){
        primitiveSelector->insertItem( static_cast< int >( primitiveID ), primitiveRelPath );
    });

    return frame;
}

} // namespace como
