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

#include "tools_menu.hpp"
#include <QVBoxLayout>

namespace como {

ToolsMenu::ToolsMenu( shared_ptr< ComoApp > comoApp )
{
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
        appModeSelector->addItem( appMode );
    }

    // When user change app mode in selector, call ComoApp::setAppMode().
    void (QComboBox::*signal)( int ) = &QComboBox::activated;
    connect( appModeSelector, signal, [=]( int index ) {
        comoApp->setAppMode( appModes[index] );
    }  );

    // When comoApp::setAppMode() be invoked, change appMode selector's index.
    connect( comoApp.get(), &ComoApp::appModeIndexChanged, appModeSelector, &QComboBox::setCurrentIndex );

    // Create and fulfill edition scope's selector.
    editionScopeGroupBox = new QGroupBox( tr( "Edition scope" ) );
    editionScopeGroupBoxLayout = new QVBoxLayout;
    for( auto editionScope : editionScopeStrings ){
        editionScopeGroupBoxLayout->addWidget( new QRadioButton( tr( (editionScope.second).c_str() ) ) );
    }
    editionScopeGroupBox->setLayout( editionScopeGroupBoxLayout );

    // Create a transformation mode's selector. The different radio buttons are copied
    // in two places:
    // 1 - In a QGroupBox, for GUI output.
    // 2 - In a QButtonGroup, for giving each button an unique id. This is used for signal
    // connecting.
    transformationModeGroupBox = new QGroupBox( tr( "Transformation mode") );
    transformationModeButtonGroup = new QButtonGroup;
    transformationModeGroupBoxLayout = new QVBoxLayout;

    // Create a QRadioButton for each transformation mode in the app. Copy the button
    // to the previous QGroupBox and QButtonGroup.
    for( unsigned int i = 0; i < transformationModeStrings.size(); i++ ){
        transformationModeRadioButton = new QRadioButton( transformationModeStrings[i] );
        transformationModeButtonGroup->addButton( transformationModeRadioButton, i );
        transformationModeGroupBoxLayout->addWidget( transformationModeRadioButton );
    }
    transformationModeGroupBox->setLayout( transformationModeGroupBoxLayout );

    // Change current transformation mode when user select it in the GUI.
    void (QButtonGroup::*buttonClicked)( int ) = &QButtonGroup::buttonClicked;
    connect( transformationModeButtonGroup, buttonClicked, [=]( int index ) {
        comoApp->setTransformationMode( transformationModes[index] );
    } );

    // Update the current checked button when the user change the current
    // transformation mode (ie. by keypress).
    connect( comoApp.get(), &ComoApp::transformationModeIndexChanged, [=]( int index ) {
        ( ( transformationModeButtonGroup->buttons() )[index] )->toggle();
    } );

    // Set tools panel layout.
    layout = new QVBoxLayout;
    layout->setSpacing( 1 );
    layout->addWidget( toolsMenuLabel );
    layout->addWidget( appModeLabel );
    layout->addWidget( appModeSelector );
    layout->addWidget( editionScopeGroupBox );
    layout->addWidget( transformationModeGroupBox );
    layout->addWidget( createPrimitiveCreationMenu() );
    setLayout( layout );
}


QGroupBox* ToolsMenu::createPrimitiveCreationMenu()
{
    QButtonGroup* primitiveCreationButtonGroup;
    QGroupBox* primitiveCreationGroupBox;
    QVBoxLayout* primitiveCreationGroupBoxLayout;
    QPushButton* primitiveCreationButton;

    // Create the containers and layouts.
    primitiveCreationGroupBox = new QGroupBox( QString::fromUtf8( "Create primitive" ) );
    primitiveCreationButtonGroup = new QButtonGroup;
    primitiveCreationGroupBoxLayout = new QVBoxLayout;

    // Add buttons to group box's layout and to buttons' group.
    for( unsigned int i = 0; i < drawableTypeStrings.size(); i++ ){
        primitiveCreationButton = new QPushButton( drawableTypeStrings[i] );

        primitiveCreationGroupBoxLayout->addWidget( primitiveCreationButton );
        primitiveCreationButtonGroup->addButton( primitiveCreationButton, i );
    }


    // Change current transformation mode when user select it in the GUI.
    void (QButtonGroup::*buttonClicked)( int ) = &QButtonGroup::buttonClicked;
    connect( primitiveCreationButtonGroup, buttonClicked, [=]( int index ) {
        comoApp->getScene()->addDrawable( drawableTypes[index] );
    } );

    /*
    // Update the current checked button when the user change the current
    // transformation mode (ie. by keypress).
    connect( comoApp.get(), &ComoApp::transformationModeIndexChanged, [=]( int index ) {
        ( ( transformationModeButtonGroup->buttons() )[index] )->toggle();
    } );
    */

    // Set the layout.
    primitiveCreationGroupBox->setLayout( primitiveCreationGroupBoxLayout );

    return primitiveCreationGroupBox;
}

} // namespace como
