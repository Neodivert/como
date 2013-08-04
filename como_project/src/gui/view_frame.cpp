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
    // Create a OpenGL canvas.
    openGLCanvas = new OpenGLCanvas( comoApp );

    // The OpenGL canvas inherits from QWindow. In order to allow it to live inside a QWidget-based
    // application, we need to create a QWidget wrapper.
    QWidget* openGLCanvasWidget = QWidget::createWindowContainer( openGLCanvas );

    // Make the OpenGL widget accept focus by both tabbing and clicking.
    openGLCanvasWidget->setFocusPolicy( Qt::StrongFocus );

    // Make the OpenGL canvas ocuppy the maximum available space.
    openGLCanvasWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    // Set a view label that takes the minimum space.
    QLabel* viewLabel = new QLabel( name );
    viewLabel->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );


    // Set app mode's selector.
    viewSelector = new QComboBox;
    for( auto viewString : viewStrings ){
        viewSelector->addItem( viewString );
    }

    // When user change view in selector, call OpenGL::setView().
    void (QComboBox::*signal)( int ) = &QComboBox::activated;
    connect( viewSelector, signal, [=]( int index ) {
        openGLCanvas->setView( views[index] );
    }  );

    // When comoApp::setAppMode() be invoked, change appMode selector's index.
    connect( openGLCanvas, &OpenGLCanvas::viewIndexChanged, viewSelector, &QComboBox::setCurrentIndex );


    // Set the ViewFrame layout.
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( viewSelector );
    layout->addWidget( openGLCanvasWidget );
    setLayout(layout);

    // When a render is requested, render!
    QObject::connect( comoApp->getScene().get(), &Scene::renderNeeded, this, &ViewFrame::render  );


}

/***
 * 2. Updating and drawing
 ***/

void ViewFrame::render()
{
    openGLCanvas->render();
}


} // namespace como
