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

#include "viewport_view_menu.hpp"

namespace como {

/***
 * 1. Construction
 ***/

ViewportViewMenu::ViewportViewMenu( Viewport* viewport, QWidget* parent ) :
    QMenu( "View", parent )
{
    unsigned int i = 0;
    QAction* currentAction = nullptr;

    for( i = 0; i < N_VIEWS; i++ ){
        currentAction = new QAction( QString( viewStrings[i] ), this );
        addAction( currentAction );

        QObject::connect( currentAction, &QAction::triggered, [=](){
            setView( static_cast< View >( i ) );
            viewport->setView( static_cast< View >( i ) );
        });
    }

    setView( viewport->getView() );
    setVisible( true );
}


/***
 * 3. Setters
 ***/

void ViewportViewMenu::setView( View view )
{
    setTitle( QString( "View: " ) + actions().at( static_cast< int >( view ) )->text() );
}

} // namespace como
