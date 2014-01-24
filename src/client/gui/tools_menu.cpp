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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "tools_menu.hpp"
#include <QVBoxLayout>

namespace como {


/***
 * 1. Initialization and destruction
 ***/

ToolsMenu::ToolsMenu( QWidget* parent, shared_ptr< ComoApp > comoApp ) :
    QFrame( parent ),
    currentColor_( 255, 0, 0, 255 )
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

    // Create a transformation mode's selector. The different radio buttons are copied
    // in two places:
    // 1 - In a QGroupBox, for GUI output.
    // 2 - In a QButtonGroup, for giving each button an unique id. This is used for signal
    // connecting.
    transformationModeGroupBox = new QGroupBox( tr( "Transformation mode") );
    transformationModeButtonGroup = new QButtonGroup( transformationModeGroupBox );
    transformationModeGroupBoxLayout = new QVBoxLayout;

    // Create a QRadioButton for each transformation mode in the app. Copy the button
    // to the previous QGroupBox and QButtonGroup.
    for( unsigned int i = 0; i < N_TRANSFORMATION_MODES; i++ ){
        transformationModeRadioButton = new QRadioButton( tr( transformationModeStrings[i] ) );
        transformationModeButtonGroup->addButton( transformationModeRadioButton, i );
        transformationModeGroupBoxLayout->addWidget( transformationModeRadioButton );
    }
    transformationModeGroupBox->setLayout( transformationModeGroupBoxLayout );

    // Change current transformation mode when user select it in the GUI.
    void (QButtonGroup::*buttonClicked)( int ) = &QButtonGroup::buttonClicked;
    connect( transformationModeButtonGroup, buttonClicked, [=]( int index ) {
        comoApp->setTransformationMode( static_cast< TransformationMode >( index ) );
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
    layout->addWidget( createPivotPointModeSelector() );
    layout->addWidget( createPrimitiveCreationMenu() );
    layout->addWidget( createColorSelector() );
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
    primitiveCreationButtonGroup = new QButtonGroup( primitiveCreationGroupBox );
    primitiveCreationGroupBoxLayout = new QVBoxLayout;

    // Add buttons to group box's layout and to buttons' group.
    for( unsigned int i = 0; i < N_DRAWABLE_TYPES; i++ ){
        primitiveCreationButton = new QPushButton( tr( drawableTypeStrings[i] ) );

        primitiveCreationGroupBoxLayout->addWidget( primitiveCreationButton );
        primitiveCreationButtonGroup->addButton( primitiveCreationButton, i );
    }

    // Signal / Slot connection: when one of the creation buttons is pressed,
    // create a drawable of the chosen type.
    void (QButtonGroup::*buttonClicked)( int ) = &QButtonGroup::buttonClicked;
    connect( primitiveCreationButtonGroup, buttonClicked, [this]( int index ) {
        Q_UNUSED( index );

        // Add one type of drawable or another to the scene according to the
        // button pressed by user.
        switch( static_cast< DrawableType >( index ) ){
            case DrawableType::CUBE:
                comoApp->getScene()->addCube( getCurrentColor() );
            break;
        }
    } );

    /*
    void (QButtonGroup::*buttonClicked)( int ) = &QButtonGroup::buttonClicked;
    connect( primitiveCreationButtonGroup, buttonClicked, [=]( int index ) {
        comoApp->getScene()->addDrawable( static_cast< DrawableType >( index ) );
    } );
    */

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


QGroupBox* ToolsMenu::createPivotPointModeSelector()
{
    QGroupBox* pivotPointModeGroupBox = nullptr;
    QVBoxLayout* pivotPointModeLayout = nullptr;
    QComboBox* pivotPointModeSelector = nullptr;

    // Create a QComboBox ("selector") with all possible pivot point modes.
    pivotPointModeSelector = new QComboBox;
    for( auto pivotPointModeString : pivotPointModeStrings ){
        pivotPointModeSelector->addItem( tr( pivotPointModeString ) );
    }

    // When user change pivot point mode in selector, call ComoApp::setPivotPoint().
    void (QComboBox::*signal)( int ) = &QComboBox::activated;
    connect( pivotPointModeSelector, signal, [=]( int index ) {
        comoApp->getScene()->setPivotPointMode( static_cast< PivotPointMode >( index ) );
    }  );

    // Set pivot point mode frame layout.
    pivotPointModeLayout = new QVBoxLayout;
    pivotPointModeLayout->addWidget( pivotPointModeSelector );

    // Set pivot point mode layout.
    pivotPointModeGroupBox = new QGroupBox( tr( "Pivot point mode" ) );
    pivotPointModeGroupBox->setLayout( pivotPointModeLayout );

    // Return the created pivot point mode selector.
    return pivotPointModeGroupBox;
}


QFrame* ToolsMenu::createColorSelector()
{
    QPushButton* selectColorButton = nullptr;
    QFrame* colorSelectorFrame = nullptr;
    QVBoxLayout* layout = nullptr;

    // Create the frame that whill hold the color's button and label.
    colorSelectorFrame = new QFrame( this );

    // Create a label showing the current selected color. The color name will
    // be colored in that color.
    currentColorLabel_ = new QLabel( "Current color: <font color=\"" + currentColor_.name() + "\">" + currentColor_.name() + "</font>" );

    // Signal / Slot connection. When the current color changes, change the
    // previous label's text accordingly.
    QObject::connect( this, &ToolsMenu::currentColorChanged, [this]( QColor newColor ){
        currentColorLabel_->setText( "Current color: <font color=\"" + newColor.name() + "\">" + newColor.name() + "</font>" );
    });

    // Create a button for changing the current color.
    selectColorButton = new QPushButton( tr( "Select color" ) );

    // Signal / Slot connection. When previous button is clicked, we invoke the
    // changeCurrentColor method.
    QObject::connect( selectColorButton, &QPushButton::clicked, this, &ToolsMenu::changeCurrentColor );

    // Set the frame's layout.
    layout = new QVBoxLayout;
    layout->addWidget( currentColorLabel_ );
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


/***
 * 3. Auxiliar methods
 ***/

void ToolsMenu::changeCurrentColor()
{
    // Open a dialog for selecting a new color.
    currentColor_ = QColorDialog::getColor( currentColor_ );

    // If the selected color is not valid, change it to red.
    if( !( currentColor_.isValid() ) ){
        currentColor_.setRgb( 255, 0, 0 );
    }

    // Emit a signal indicating that the current color has changed.
    emit currentColorChanged( currentColor_ );
}

} // namespace como
