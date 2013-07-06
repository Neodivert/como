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


ViewFrame::ViewFrame( const QString &name, shared_ptr<OpenGLContext> glContext, shared_ptr<Scene> scene ) :
    QFrame()
{
    QHBoxLayout *layout = new QHBoxLayout;

    QWidget *container = QWidget::createWindowContainer( new OpenGLCanvas( glContext, scene ) );
    container->setFocusPolicy( Qt::StrongFocus );
    container->setAcceptDrops( true );
    layout->addWidget( container );
    setLayout(layout);
}
