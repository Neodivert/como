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

#include "transformation_tab.hpp"
#include <QGroupBox>
#include <QButtonGroup>
#include <QLayout>

namespace como {

TransformationTab::TransformationTab( ComoAppPtr comoApp ) :
    comoApp_( comoApp )
{
    // Create a layout for this frame.
    QVBoxLayout* layout = new QVBoxLayout;

    // Add menus and widgets to the previous layout.
    layout->addWidget( createTransformationLockMenu() );
    layout->addWidget( createPivotPointModeSelector() );

    // Set previous layout as the current one for this tab.
    setLayout( layout );
}


/***
 * 2. Initialization
 ***/

QGroupBox* TransformationTab::createTransformationLockMenu()
{
    QGroupBox* groupBox = nullptr;
    QButtonGroup* buttonGroup = nullptr;
    QVBoxLayout* layout = nullptr;
    QRadioButton* radioButton = nullptr;

    // Create a transformation mode's selector. The different radio buttons are copied
    // in two places:
    // 1 - In a QGroupBox, for GUI output.
    // 2 - In a QButtonGroup, for giving each button an unique id. This is used for signal
    // connecting.
    groupBox = new QGroupBox( tr( "Transformation lock mode") );
    buttonGroup = new QButtonGroup( groupBox );
    layout = new QVBoxLayout;

    // Create a QRadioButton for each transformation mode in the app. Copy the button
    // to the previous QGroupBox and QButtonGroup.
    for( unsigned int i = 0; i < N_TRANSFORMATION_MODES; i++ ){
        radioButton = new QRadioButton( tr( transformationModeStrings[i] ) ); // TODO: Change names in ComoApp class from transformationMode* to transformationLockMode*.
        buttonGroup->addButton( radioButton, i );
        layout->addWidget( radioButton );
    }
    groupBox->setLayout( layout );

    // Change current transformation mode when user select it in the GUI.
    void (QButtonGroup::*buttonClicked)( int ) = &QButtonGroup::buttonClicked;
    connect( buttonGroup, buttonClicked, [=]( int index ) {
        comoApp_->setTransformationMode( static_cast< TransformationMode >( index ) );
    } );

    // Update the current checked button when the user change the current
    // transformation mode (ie. by keypress).
    connect( comoApp_.get(), &ComoApp::transformationModeIndexChanged, [=]( int index ) {
        ( ( buttonGroup->buttons() )[index] )->toggle();
    } );

    return groupBox;
}


QGroupBox* TransformationTab::createPivotPointModeSelector()
{
    QGroupBox* groupBox = nullptr;
    QVBoxLayout* layout = nullptr;
    QComboBox* selector = nullptr;

    // Create a QComboBox ("selector") with all possible pivot point modes.
    selector = new QComboBox;
    for( auto pivotPointModeString : pivotPointModeStrings ){
        selector->addItem( tr( pivotPointModeString ) );
    }

    // When user change pivot point mode in selector, call ComoApp::setPivotPoint().
    void (QComboBox::*signal)( int ) = &QComboBox::activated;
    connect( selector, signal, [this]( int index ){
        (void)( index );
        // TODO: Implement: comoApp_->getScene()->getEntitiesManager()->getLocalResourcesSelection()->setPivotPointMode( static_cast< PivotPointMode >( index ) );
    }  );

    // Set pivot point mode frame layout.
    layout = new QVBoxLayout;
    layout->addWidget( selector );

    // Set pivot point mode layout.
    groupBox = new QGroupBox( tr( "Pivot point mode" ) );
    groupBox->setLayout( layout );

    // Return the created pivot point mode selector.
    return groupBox;
}

} // namespace como
