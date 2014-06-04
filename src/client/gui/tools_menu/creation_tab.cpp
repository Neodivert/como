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

#include "creation_tab.hpp"

namespace como {

/***
 * 1. Construction
 ***/

CreationTab::CreationTab( ScenePtr scene ) :
    scene_( scene )
{
    // Create a layout for this tab.
    QVBoxLayout* layout = new QVBoxLayout;

    // Add widgets to the previous layout.
    layout->addWidget( createMeshFromPrimitiveCreationMenu() );

    // Set the previous layout as this tab's layout.
    setLayout( layout );
}


/***
 * 2. Initialization
 ***/

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
    connect( primitiveSelector, activated, [=]( int primitiveID ) {
        scene_->getDrawablesManager()->createMeshAndMaterial( static_cast< PrimitiveID >( primitiveID ) );
    });

    // Signal / Slot connection: when a new primitive is created in the scene,
    // add it to the primitives dropdown list.
    connect( scene_.get(), &Scene::primitiveAdded, [=]( const QString& primitiveRelPath, PrimitiveID primitiveID ){
        primitiveSelector->insertItem( static_cast< int >( primitiveID ), primitiveRelPath );
    });

    return frame;
}

} // namespace como
