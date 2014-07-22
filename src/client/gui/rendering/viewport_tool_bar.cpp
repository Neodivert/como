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

#include "viewport_tool_bar.hpp"

namespace como {


/***
 * 1. Construction
 ***/

ViewportToolBar::ViewportToolBar( Viewport* viewport, View currentView ) :
    QToolBar( nullptr )
{
    QAction* currentAction = nullptr;

    currentAction = new QAction( QString( "Maximize" ), nullptr );
    currentAction->setCheckable( true );
    QObject::connect( currentAction, &QAction::triggered, [this]( bool checked ){
        if( checked ){
            emit viewFrameMaximizationRequested();
        }else{
            emit viewFrameMinimizationRequested();
        }
    });
    addAction( currentAction );

    addWidget( createViewSelector( viewport, currentView ) );
}


/***
 * 2. Initialization
 ***/

QComboBox* ViewportToolBar::createViewSelector( Viewport* viewport, View currentView ) const
{
    QComboBox* viewSelector = new QComboBox;

    for( auto viewString : viewStrings ){
        viewSelector->addItem( tr( viewString ) );
    }

    // Select the given view.
    viewSelector->setCurrentIndex( static_cast< int >( currentView ) );

    // When user change view in selector, call Viewport::setView().
    void (QComboBox::*signal)( int ) = &QComboBox::activated;
    connect( viewSelector, signal, [=]( int index ) {
        viewport->setView( static_cast< View >( index ) );
    });

    return viewSelector;
}

} // namespace como
