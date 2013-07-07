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


ViewFrame::ViewFrame( const QString &name, shared_ptr<QOpenGLContext> glContext, shared_ptr<Scene> scene ) :
    QFrame()
{
    // Creates a QWidget wrapper for an existing QWindow, allowing it to live inside
    // a QWidget-based application.
    QWidget* openGLCanvasWidget = QWidget::createWindowContainer( new OpenGLCanvas( glContext, scene ) );
    openGLCanvasWidget->setFocusPolicy( Qt::StrongFocus );
    //openGLCanvasWidget->setAcceptDrops( true );

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget( openGLCanvasWidget );
    setLayout(layout);
}
