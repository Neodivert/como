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

#include "view_frame.hpp"

namespace como {

/***
 * 1. Initialization
 ***/

ViewFrame::ViewFrame( const QString &name, shared_ptr< ComoApp > comoApp ) :
    QFrame()
{
    QFrame* header;
    QHBoxLayout* headerLayout;

    cout << "ViewFrame 1" << endl;
    showError();

    // Create a OpenGL viewport.
    viewport = new Viewport( comoApp );

    cout << "ViewFrame 2" << endl;
    showError();

    // The viewport inherits from QWindow. In order to allow it to live inside a QWidget-based
    // application, we need to create a QWidget wrapper.
    QWidget* viewportWidget = QWidget::createWindowContainer( viewport );

    // Make the viewport widget accept focus by both tabbing and clicking.
    viewportWidget->setFocusPolicy( Qt::StrongFocus );

    // Make the viewport ocuppy the maximum available space.
    viewportWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    // Set a view label that takes the minimum space.
    QLabel* viewLabel = new QLabel( name );
    viewLabel->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

    // Set a dropdown list for selecting the viewport's current view.
    viewSelector = new QComboBox;
    for( auto viewString : viewStrings ){
        viewSelector->addItem( viewString );
    }

    // When user change view in selector, call Viewport::setView().
    void (QComboBox::*signal)( int ) = &QComboBox::activated;
    connect( viewSelector, signal, [=]( int index ) {
        viewport->setView( views[index] );
    }  );

    // When comoApp::setAppMode() be invoked, change appMode selector's index.
    connect( viewport, &Viewport::viewIndexChanged, viewSelector, &QComboBox::setCurrentIndex );

    //
    projectionModeSwitch = createProjectionSwitch();

    // Set the ViewFrame's header layout.
    header = new QFrame;
    headerLayout = new QHBoxLayout;
    headerLayout->addWidget( viewSelector );
    headerLayout->addWidget( projectionModeSwitch );
    header->setLayout( headerLayout );

    // Set the ViewFrame layout.
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( header );
    layout->addWidget( viewportWidget );
    setLayout(layout);

    // When a render is requested, render!
    QObject::connect( comoApp->getScene().get(), &Scene::renderNeeded, this, &ViewFrame::render  );
}


QGroupBox* ViewFrame::createProjectionSwitch()
{
    QGroupBox* projectionModeGroupBox;
    QButtonGroup* projectionModeButtonGroup;
    QVBoxLayout* projectionModeGroupBoxLayout;
    QRadioButton* projectionModeRadioButton;

    // Create a projection mode's selector. The different radio buttons are copied
    // in two places:
    // 1 - In a QGroupBox, for GUI output.
    // 2 - In a QButtonGroup, for giving each button an unique id. This is used for signal
    // connecting.
    projectionModeGroupBox = new QGroupBox( tr( "Projection mode") );
    projectionModeButtonGroup = new QButtonGroup;
    projectionModeGroupBoxLayout = new QVBoxLayout;

    // Create a QRadioButton for each projection mode in the app. Copy the button
    // to the previous QGroupBox and QButtonGroup.
    for( unsigned int i = 0; i < projectionModeStrings.size(); i++ ){
        projectionModeRadioButton = new QRadioButton( projectionModeStrings[i] );
        projectionModeButtonGroup->addButton( projectionModeRadioButton, i );
        projectionModeGroupBoxLayout->addWidget( projectionModeRadioButton );
    }
    projectionModeGroupBox->setLayout( projectionModeGroupBoxLayout );

    // Toggle first projection mode option (ortho).
    ( ( projectionModeButtonGroup->buttons() )[0] )->toggle();

    // Change current projection mode when user select it in the GUI.
    void (QButtonGroup::*buttonClicked)( int ) = &QButtonGroup::buttonClicked;
    connect( projectionModeButtonGroup, buttonClicked, [=]( int index ) {
        viewport->setProjection( projectionModes[index] );
    } );

    // Update the current checked button when the user change the current
    // projection mode (ie. by keypress).
    //connect( comoApp.get(), &ComoApp::transformationModeIndexChanged, [=]( int index ) {
    //    ( ( transformationModeButtonGroup->buttons() )[index] )->toggle();
    //} );

    return projectionModeGroupBox;
}

/***
 * 2. Updating and drawing
 ***/

void ViewFrame::render()
{
    viewport->render();
}


} // namespace como
