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

#include <QVBoxLayout>
#include "view_frame.hpp"


namespace como {

/***
 * 1. Initialization
 ***/

ViewFrame::ViewFrame( View view, Projection projection, shared_ptr< ComoApp > comoApp ) :
    QFrame()
{
    // Create a OpenGL viewport and check OpenGL state.
    viewport = new Viewport( view, projection, comoApp );

    // The viewport inherits from QWindow. In order to allow it to live inside a QWidget-based
    // application, we need to create a QWidget wrapper.
    QWidget* viewportWidget = QWidget::createWindowContainer( viewport );

    // Make the viewport widget accept focus by both tabbing and clicking.
    viewportWidget->setFocusPolicy( Qt::StrongFocus );

    // Make the viewport ocuppy the maximum available space.
    viewportWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    toolbar_ = new ViewportToolBar( viewport );
    toolbar_->setVisible( true );

    // When requested, forward the "viewFrameMaximizationRequested" and
    // "viewFrameMinimizationnRequested" signals to the outside.
    QObject::connect( toolbar_, &ViewportToolBar::viewFrameMaximizationRequested, [this](){
        emit viewFrameMaximizationRequested( this );
    });
    QObject::connect( toolbar_, &ViewportToolBar::viewFrameMinimizationRequested, [this](){
        emit viewFrameMinimizationRequested();
    });

    // Set the ViewFrame layout.
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( toolbar_ );
    layout->addWidget( viewportWidget );
    setLayout(layout);
}


/***
 * 3. Updating and drawing
 ***/

void ViewFrame::renderIfNeeded()
{
    render();
}

void ViewFrame::render()
{
    OpenGL::checkStatus( "ViewFrame::render - begin" );
    viewport->render();
    OpenGL::checkStatus( "ViewFrame::render - end" );
}


} // namespace como
