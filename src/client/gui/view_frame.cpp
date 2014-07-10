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

#include "view_frame.hpp"

namespace como {

/***
 * 1. Initialization
 ***/

ViewFrame::ViewFrame( View view, shared_ptr< ComoApp > comoApp ) :
    QFrame()
{
    QFrame* header;
    QHBoxLayout* headerLayout;

    // Create a OpenGL viewport and check OpenGL state.
    viewport = new Viewport( view, comoApp );

    // The viewport inherits from QWindow. In order to allow it to live inside a QWidget-based
    // application, we need to create a QWidget wrapper.
    QWidget* viewportWidget = QWidget::createWindowContainer( viewport );

    // Make the viewport widget accept focus by both tabbing and clicking.
    viewportWidget->setFocusPolicy( Qt::StrongFocus );

    // Make the viewport ocuppy the maximum available space.
    viewportWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    // Set a dropdown list for selecting the viewport's current view.
    viewSelector = new QComboBox;
    for( auto viewString : viewStrings ){
        viewSelector->addItem( tr( viewString ) );
    }

    // Select the given view.
    viewSelector->setCurrentIndex( static_cast< int >( view ) );

    // When user change view in selector, call Viewport::setView().
    void (QComboBox::*signal)( int ) = &QComboBox::activated;
    connect( viewSelector, signal, [=]( int index ) {
        viewport->setView( static_cast< View >( index ) );
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
    OpenGL::checkStatus( "Viewframe - Constructor" );
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
    projectionModeButtonGroup = new QButtonGroup( projectionModeGroupBox );
    projectionModeGroupBoxLayout = new QVBoxLayout;

    // Create a QRadioButton for each projection mode in the app. Copy the button
    // to the previous QGroupBox and QButtonGroup.
    for( unsigned int i = 0; i < N_PROJECTIONS; i++ ){
        projectionModeRadioButton = new QRadioButton( tr( projectionModeStrings[i] ) );
        projectionModeButtonGroup->addButton( projectionModeRadioButton, i );
        projectionModeGroupBoxLayout->addWidget( projectionModeRadioButton );
    }
    projectionModeGroupBox->setLayout( projectionModeGroupBoxLayout );

    // Toggle first projection mode option (ortho).
    ( ( projectionModeButtonGroup->buttons() )[0] )->toggle();

    // Change current projection mode when user select it in the GUI.
    void (QButtonGroup::*buttonClicked)( int ) = &QButtonGroup::buttonClicked;
    connect( projectionModeButtonGroup, buttonClicked, [=]( int index ) {
        viewport->setProjection( static_cast< Projection >( index ) );
    } );

    // Update the current checked button when the user change the current
    // projection mode (ie. by keypress).
    //connect( comoApp.get(), &ComoApp::transformationModeIndexChanged, [=]( int index ) {
    //    ( ( transformationModeButtonGroup->buttons() )[index] )->toggle();
    //} );

    return projectionModeGroupBox;
}

/***
 * 3. Updating and drawing
 ***/

void ViewFrame::renderIfNeeded()
{
    viewport->renderIfNeeded();
}


} // namespace como
