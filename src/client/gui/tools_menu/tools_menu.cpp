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

#include "tools_menu.hpp"
#include "creation_tab.hpp"
#include "transformation_tab.hpp"
#include <QVBoxLayout>

namespace como {


/***
 * 1. Initialization and destruction
 ***/

ToolsMenu::ToolsMenu( QWidget* parent, shared_ptr< ComoApp > comoApp ) :
    QTabWidget( parent ),
    currentColor_( 255, 0, 0, 255 ),
    currentDirectionalLightColor_( 255, 0, 0, 255 )
{
    addTab( new CreationTab( comoApp->getScene() ), "Creation" );
    addTab( new TransformationTab( comoApp ), "Transformation" );

    /*
    QVBoxLayout* layout;
    QLabel* toolsMenuLabel;
    QLabel* appModeLabel;
    QComboBox* appModeSelector;
    QGroupBox* editionScopeGroupBox;
    QVBoxLayout* editionScopeGroupBoxLayout;

    QGroupBox* transformationModeGroupBox;
    QButtonGroup* transformationModeButtonGroup;
    QVBoxLayout* transformationModeGroupBoxLayout;
    QRadioButton* transformationModeRadioButton;

    // Share the given pointer to app's state.
    this->comoApp = comoApp;

    // Set tools menu's label.
    toolsMenuLabel = new QLabel( "Tools menu" );

    // Set app mode's label.
    appModeLabel = new QLabel( "App mode" );

    // Set app mode's selector.
    appModeSelector = new QComboBox;
    for( auto appMode : appModeStrings ){
        appModeSelector->addItem( tr( appMode ) );
    }

    // When user change app mode in selector, call ComoApp::setAppMode().
    void (QComboBox::*signal)( int ) = &QComboBox::activated;
    connect( appModeSelector, signal, [=]( int index ) {
        comoApp->setAppMode( static_cast< AppMode >( index ) );
    }  );

    // When comoApp::setAppMode() be invoked, change appMode selector's index.
    connect( comoApp.get(), &ComoApp::appModeIndexChanged, appModeSelector, &QComboBox::setCurrentIndex );

    // Create and fulfill edition scope's selector.
    editionScopeGroupBox = new QGroupBox( tr( "Edition scope" ) );
    editionScopeGroupBoxLayout = new QVBoxLayout;
    for( auto editionScope : editionScopeStrings ){
        editionScopeGroupBoxLayout->addWidget( new QRadioButton( tr( editionScope ) ) );
    }
    editionScopeGroupBox->setLayout( editionScopeGroupBoxLayout );

    // Set tools panel layout.
    layout = new QVBoxLayout;
    layout->setSpacing( 1 );
    layout->addWidget( toolsMenuLabel );
    layout->addWidget( appModeLabel );
    layout->addWidget( appModeSelector );
    layout->addWidget( editionScopeGroupBox );
    layout->addWidget( transformationModeGroupBox );
    layout->addWidget( createPivotPointModeSelector() );
    layout->addWidget( createPrimitiveCreationMenu() );
    layout->addWidget( createColorSelector() );
    layout->addWidget( createDirectionalLightColorSelector() );
    setLayout( layout );
    */
}


QFrame* ToolsMenu::createDirectionalLightColorSelector()
{
    QPushButton* selectColorButton = nullptr;
    QFrame* colorSelectorFrame = nullptr;
    QVBoxLayout* layout = nullptr;
    QLabel* currentColorLabel = nullptr;

    // Create the frame that whill hold the color's button and label.
    colorSelectorFrame = new QFrame( this );

    // Create a label showing the current selected color. The color name will
    // be colored in that color.
    currentColorLabel = new QLabel( "Current ambient light color: <font color=\"" + currentDirectionalLightColor_.name() + "\">" + currentDirectionalLightColor_.name() + "</font>" );

    // Signal / Slot connection. When the current color changes, change the
    // previous label's text accordingly.
    QObject::connect( this, &ToolsMenu::currentDirectionalLightColorChanged, [=]( QColor newColor ){
        currentColorLabel->setText( "Current color: <font color=\"" + newColor.name() + "\">" + newColor.name() + "</font>" );

        comoApp->getScene()->setDirectionalLightColor( glm::vec3( newColor.red() / 255.0f, newColor.green() / 255.0f, newColor.blue() / 255.0f ) );
    });

    // Create a button for changing the current color.
    selectColorButton = new QPushButton( tr( "Select color" ) );

    // Signal / Slot connection. When previous button is clicked, we invoke the
    // changeCurrentColor method.
    QObject::connect( selectColorButton, &QPushButton::clicked, [this](){
        // Open a dialog for selecting a new color.
        currentDirectionalLightColor_ = QColorDialog::getColor( currentDirectionalLightColor_ );

        // If the selected color is not valid, change it to red.
        if( !( currentDirectionalLightColor_.isValid() ) ){
            currentDirectionalLightColor_.setRgb( 255, 0, 0 );
        }

        // Emit a signal indicating that the current color has changed.
        emit currentDirectionalLightColorChanged( currentDirectionalLightColor_ );
    });

    // Set the frame's layout.
    layout = new QVBoxLayout;
    layout->addWidget( currentColorLabel );
    layout->addWidget( selectColorButton );
    colorSelectorFrame->setLayout( layout );

    // Return the frame.
    return colorSelectorFrame;
}


/***
 * 2. Getters
 ***/

QColor ToolsMenu::getCurrentColor() const
{
    return currentColor_;
}




} // namespace como
