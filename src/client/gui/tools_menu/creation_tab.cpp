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
#include <QLabel>

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
    layout->addWidget( new SystemPrimitivesCreationMenu( scene->getSystemPrimitivesFactory() ) );
    layout->addWidget( createMeshFromPrimitiveCreationMenu() );
    layout->addWidget( createPrimitiveImportButton() );
    layout->addWidget( createLightsCreationPanel() );

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
    ResourceSelector* primitiveSelector = nullptr;
    QLabel* label = nullptr;

    // Create an empty dropdown list (QComboBox).
    primitiveSelector = new ResourceSelector;

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
    QObject::connect( primitiveSelector, &ResourceSelector::resourceSelected, [this]( ResourceID primitiveID ){
        try {
            scene_->getPrimitivesManager()->instantiatePrimitive( primitiveID );
        }catch( std::exception& ex ){
            std::string errorMessage =
                    "Error instantiating primitive [" +
                    scene_->getPrimitivesManager()->getPrimitiveInfo( primitiveID ).filePath +
                    "]: " +
                    ex.what();

            QMessageBox::critical( nullptr,
                                   "Error instantiating primitive",
                                   errorMessage.c_str() );
        }
    });


    // Signal / Slot connection: when a new primitive is created in the scene,
    // add it to the primitives dropdown list.
    QObject::connect( scene_->getPrimitivesManager().get(), &ClientPrimitivesManager::primitiveAdded, [=]( ResourceID primitiveID, std::string primitiveRelPath ){
        primitiveSelector->insertResource( primitiveID, primitiveRelPath );
    });

    return frame;
}


QPushButton* CreationTab::createPrimitiveImportButton() const
{
    QPushButton* importPrimitiveButton = new QPushButton( "Import primitive" );

    QObject::connect( importPrimitiveButton, &QPushButton::clicked, [this](){
        PrimitiveImportDialog primitiveImportDialog( scene_->getPrimitivesManager().get() );

        primitiveImportDialog.exec();
    });

    return importPrimitiveButton;
}


QFrame *CreationTab::createLightsCreationPanel() const
{
    QFrame* lightsCreationFrame = new QFrame;

    QPushButton* createDirectionalLightButton =
            new QPushButton( "Create directional light" );

    QObject::connect( createDirectionalLightButton, &QPushButton::pressed, [this](){
        scene_->getEntitiesManager()->getLightsManager()->requestDirectionalLightCreation();
    });

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget( new QLabel( "Lights creation" ) );
    layout->addWidget( createDirectionalLightButton );
    lightsCreationFrame->setLayout( layout );

    return lightsCreationFrame;
}

} // namespace como
