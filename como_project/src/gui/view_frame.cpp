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

    // Set a title label that takes the minimum space.
    QLabel* titleLabel = new QLabel( name );
    titleLabel->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

    // Set the ViewFrame layout.
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( titleLabel );
    layout->addWidget( openGLCanvasWidget );
    setLayout(layout);
}
